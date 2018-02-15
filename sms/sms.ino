#include <SoftwareSerial.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <EdgeDetection.h>
#include <MemoryFree.h>
#include "readVcc.h"
#include "StateChangeDetection.h"

#define I2C_ADDR    0x27  // Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
#include "printBar.h"
const byte buttonPin = 5;    // the pin that the pushbutton is attached to
const byte tempPin = 0;
const byte heatingPin = 6;

uint8_t degree[8] = {
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
};

#include "DHT.h"

#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define trigPin 13
#define echoPin 12

SoftwareSerial modemSerial(2, 3); // RX, TX
EdgeDetection heating(heatingPin);
//String msg;
//short incomingByte;
short previousMillis = 0; 
const short interval = 1000;
short lastState = 0;
short currentMillis;
boolean modem = false;
const String mojNumer = "+48507133471";
#include "DHT_11.h"
#include "HC-SR04.h"
#include "LM35.h"
#include "mVcc.h"
#include "sms_S.h"
#include "smsCmd.h"

void setup() {
  lcd.begin (16,2);
  lcd.createChar(byte(0), degree);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);  // Switch on the backlight LCD
  lcd.home();
  lcd.print("   Loading...");
  
  Serial.begin(115200);
  delay(200);  byte i = 0; char c = char(255);lcd.setCursor(0,1);lcd.print(c);
  modemSerial.begin(9600);  
  delay(200);lcd.print(c);
  Serial.print("freeMemory()=");Serial.println(freeMemory());lcd.print(c);
  
  pinMode(buttonPin, INPUT_PULLUP);lcd.print(c);  // initialize the button pin as a input
  analogReference(INTERNAL);lcd.print(c);

  Serial.println("DHTxx test!");lcd.print(c);
  dht.begin();lcd.print(c);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);lcd.print(c);
  pinMode(heatingPin, INPUT);

  String at_response;lcd.print(c);
  at_response = sendCommand("AT", 500);
  if (at_response.substring(at_response.length()-2) == "OK"){
    Serial.println("Modem connected...");lcd.print(c);
    at_response = sendCommand("AT&F", 500);
    if (at_response.substring(at_response.length()-2) == "OK"){
      Serial.println("Modem reset...");lcd.print(c);
    }
    at_response = sendCommand("ATE=0", 500);
    if (at_response.substring(at_response.length()-2) == "OK") {
      Serial.println("Non-echo mode set...");lcd.print(c);
    }    
    at_response = sendCommand("AT+CMGF=0", 500);
    if (at_response.substring(at_response.length()-2) == "OK") {
      Serial.println("PDU mode set...");lcd.print(c);
    }
    at_response = sendCommand("AT+CPMS=\"ME\",\"ME\"",500);
    if (at_response.substring(at_response.length()-2) == "OK") {
      Serial.println("Messages saved to phone memory set...");lcd.print(c);
    }
    at_response = sendCommand("AT+CNMI=,1",500);       
    if (at_response.substring(at_response.length()-2) == "OK") {
      Serial.println("Messages routed to terminal...");lcd.print(c);
    }
    Serial.println("Ready to go...");
    //setNr("+48507133471");
    //setMsg("System zostal uruchomiony");
    sendSms(mojNumer, "System zostal uruchomiony");lcd.print(c);
    modem = true; 
    delay(1000);
  }
  lcd.clear();
  Serial.print("freeMemory = ");Serial.println(freeMemory());
}

void loop() {
  Serial.println(freeMemory());
  currentMillis = millis();
  switch (buttonPushCounter) {
  case 0:
    lm35Screen();
    break;
  case 1:
    dhtScreen();
    break;
  case 2:
    sr04Screen();
    break;
  case 3:
    mvccScreen();
    break;
  }
  StateChangeDetection(buttonPin);
  if(!(lastState == buttonPushCounter)){
    lastState = buttonPushCounter;
    lcd.clear();
  }
  if(modem) smsCmd();
  delay(1);  // delay in between reads for stability
}
