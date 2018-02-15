//Code written by Martin Thalheimer, 2012

/*
 This is a code which enables an Arduino board to send SMS messages in PDU format. 
 It was tested with a Siemens C45 mobile phone, but should work also with other models
 which have a built-in modem and a serial interface.
 */

//String message; //here you can type in the text you want to send
//String phone_nr=("+48507133471"); //recipient's phone number in international format
//byte original_byte;
//byte newbyte;
//byte pdu_length;
//int counter;
//String newphone_nr;
//int numlength;

void sendSms(String phone_nr, String message){
  byte original_byte;
  byte newbyte;
  byte pdu_length;
  byte numlength;
  byte counter;
  String newphone_nr;
  //lcd.clear();lcd.print("  Wysylanie...  ");
//  Serial.println("Prepare sms...");
  numlength=phone_nr.length()-1;
  if (numlength % 2 != 0 ) {
    phone_nr += "F"; 
  }
  for(int x = 0; x < numlength; x = x+2){
    newphone_nr.concat(phone_nr.charAt(x+2));
    newphone_nr.concat(phone_nr.charAt(x+1));
  }
//  Serial.println("Nr telefonu:");Serial.println(newphone_nr);
//  Serial.println("Tresc wiadomosci:");Serial.println(message);Serial.println("----------");
  //start sending SMS
  int textlength = message.length()*7/8;   // text message length: number of characters*7/8
  if (message.length()*7%8 != 0) {
    textlength++;
  } 
  if (numlength % 2 != 0) {   //if the length of phone number was uneven, it now increases by 1 because of the added "F"
    pdu_length = (8 + (numlength + 1) / 2 + textlength);
  } 
  else { //8 bytes of parameters + (length of phonenumber)/2 + number of bytes in the text
    pdu_length = (8 + numlength/2 + textlength);
  }

  modemSerial.print("AT+CMGS="); //AT+CMGW - message will be stored in the mobile phone - AT+CMGS will send it off.
//  Serial.print("AT+CMGS=");
  modemSerial.println(pdu_length);
//  Serial.println(pdu_length);
  delay(1000);
  modemSerial.print("001100");  //these are some parameters which we don't need to bother about
//  Serial.print("001100");
  if (numlength<16){       //now we have to send the byte containing the length of the phone number in HEX format
    modemSerial.print(0);
//    Serial.print(0);
  }        //the byte in HEX format needs to be 2 characters long; if needed we add a 0 in front, e.g. "C" will become "0C"
  modemSerial.print(numlength,HEX); //length telnr - this time any additional F is not to be considered
//  Serial.print(numlength,HEX);
  modemSerial.print("91");     //international format of phone number //81 for national format
//  Serial.print("91");
  modemSerial.print(newphone_nr); //now we send the rearranged phone number
//  Serial.print(newphone_nr);
  modemSerial.print("0000AA"); //some more parameters 
//  Serial.print("0000AA");
  if (message.length()<16){
    modemSerial.print(0);
//    Serial.print(0);
  }       //a byte in HEX format indicating the length of the text. Again the byte needs to be made of 2 characters
  modemSerial.print(message.length(),HEX);
//  Serial.print(message.length(),HEX); 

  counter=0;
  newbyte=0;
  for (int j=0; j<message.length(); j++) {
    original_byte = message.charAt(j); //one by one we take each byte out of the original text

    for (int i=0; i<7; i++) {
      newbyte=newbyte|bitRead(original_byte, i)<<counter;  //take the bits 0 to 6 of the old byte, shift as required for the new byte and add to the new byte
      counter++;
      if (counter==8) {
        counter=0;          //if a new byte is complete, set the counter to 0
        if (newbyte<16){
          modemSerial.print(0);
//          Serial.print(0);
        } //each byte in HEX format should consist of 2 characters, if necessary add 0 in front
        modemSerial.print(newbyte, HEX); //when the new byte is finished, send it to the mobile phone
//        Serial.print(newbyte, HEX);
        newbyte=0; // the next new byte will start again from 0
      }
    }
  }
  if (message.length()*7%8!=0) {    //if there were remaining bits (not enough to form a new byte), these are sent out as the last byte
    if (newbyte<16){
      modemSerial.print(0);
//      Serial.print(0);
    } //each byte in HEX format should consist of 2 characters, if necessary add 0 in front
    modemSerial.print(newbyte, HEX);
//    Serial.print(newbyte, HEX);
  }
  delay (100);
  modemSerial.write(26); //this is a command telling the mobile phone that the end of the transmission has been reached
  message="";
  Serial.println(freeMemory());
//  Serial.print("Wysylanie...");

  String msg;
  while (modemSerial.isListening() > 0){
    if(modemSerial.available() >0){
      msg += char(modemSerial.read());
      Serial.println(msg);
      //Serial.print(incomingByte, DEC);//;Serial.println((char)incomingByte);
      //Serial.print("freeMemory()=");Serial.println(freeMemory());
    }
    //Serial.println(msg);
    
    //if (-1 != msg2.indexOf("OK")){//7975 OK
    if(msg.substring(msg.length()-2) == "OK"){
      //lcd.print("wyslano");
//      Serial.println("ok");
      //return 0;
      break;
    }
    //if(-1 != msg2.indexOf("ERROR")){//ERROR 6982827982
    //if(msg.substring(msg.length()-5) == "ERROR"){
    if(msg == "ERROR"){
//      Serial.println("error");
      //Serial.print(msg2);
      while (modemSerial.available() > 0){
        Serial.write(modemSerial.read());
      }
      //return 1;
      break;
    }
    if(msg.length() == 5){
      msg = msg.substring(msg.length() - 4);
    }
  }
}

String sendCommand(String at_cmd, int dly){

  String msg;
  short start = millis();
  modemSerial.println(at_cmd);
  while (modemSerial.isListening() > 0){
    if(modemSerial.available() >0){
      msg += char(modemSerial.read());
      //Serial.println(msg);
    }
    if (msg.substring(msg.length()-2) == "OK"){
      break;
      // retunt msg;
    }
    else if(millis()-start > dly){
//      Serial.println("Time out command or error");
      break;
      //return -1
    }
  }
  //Serial.println(msg);
  //Serial.println(msg.substring(msg.length()-2));
  return msg;// del
}

/*
+CME ERROR: 16 Incorrect password
+CME ERROR: 17 SIM PIN2 required
+CME ERROR: 18 SIM PUK2 required
+CME ERROR: 20 Memory full
+CME ERROR: 21 Invalid index
+CME ERROR: 22 Not found
+CME ERROR: 23 Memory failure
+CME ERROR: 24 Text string too long
+CME ERROR: 25 Invalid character in text string
+CME ERROR: 26 Dial string too long
+CME ERROR: 27 Invalid characters in dial string
+CME ERROR: 30 No network service
+CME ERROR: 31 Network timeout
+CME ERROR: 32 Network not allowed – emergency calls only
+CME ERROR: 40 Network personalisation PIN required
+CME ERROR: 41 Network personalisation PUK required
+CME ERROR: 42 Network subset personalisation PIN required
+CME ERROR: 43 Network subset personalisation PUK required
+CME ERROR: 44 Service provider personalisation PIN required
+CME ERROR: 45 Service provider personalisation PUK required
+CME ERROR: 46 Corporate personalisation PIN required
+CME ERROR: 47 Corporate personalisation PUK required
+CME ERROR: 100 Unknown

+CMS ERROR: 0...127 GSM 04.11 Annex E-2 values
+CMS ERROR: 128...255 GSM 03.40 Section 9.2.3.22 values
+CMS ERROR: 300 ME failure
+CMS ERROR: 301 SMS service of ME reserved
+CMS ERROR: 302 Operation not allowed
+CMS ERROR: 303 Operation not supported
+CMS ERROR: 304 Invalid PDU mode parameter
+CMS ERROR: 305 Invalid text mode parameter
+CMS ERROR: 310 (U)SIM card not inserted
+CMS ERROR: 311 (U)SIM PIN required
+CMS ERROR: 312 PH-(U)SIM PIN required
+CMS ERROR: 313 (U)SIM card failure
*/
