#include <EdgeDetection.h>
const byte buttonPin = 5;
EdgeDetection button(buttonPin);
void setup(){
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
}

void loop(){
  byte change = button.edge();
  if(change == 1){
    Serial.print("buttonPin changed its state from LOW to HIGH");
  }
  else if(change == 0){
    Serial.print("buttonPin changed its state from HIGH to LOW");
  }
  else{
    Serial.print("not detected changes");
  }
  delay(200);
  
}

