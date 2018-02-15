
void usunWiadomosc(char c){
  String msg;
//  Serial.print("Kasowanie wiadomosci...");
  modemSerial.print("AT+CMGD=");
  modemSerial.println(c);
  while (modemSerial.isListening() > 0){
    if(modemSerial.available() >0){
      msg += (char)modemSerial.read();
    }
    if(-1 != msg.indexOf("OK")){
//      Serial.println("ok");
      break;
    }
  }
}

void smsCmd(){
  //Serial.print("freeMemory(smsCmd)=");Serial.println(freeMemory());
  modemSerial.listen();
  String msg;
  while (modemSerial.available() > 0) {
    //msg.concat(char(modemSerial.read()));
    msg += (char)modemSerial.read();
    //Serial.println(freeMemory());
    //Serial.println(msg);
  }
  //+CMTI: "ME",1
  if(-1 != msg.indexOf("+CMTI:")){
    //Serial.println("New message...");
    char c = msg.charAt(msg.indexOf("+CMTI:")+12);
    //Serial.print("Numer SMSa: ");Serial.println(c);

    //  Serial.print("Czytanie wiadomosci...");
      modemSerial.print("AT+CMGR=");modemSerial.println(c);
      while (modemSerial.isListening() > 0){
        if(modemSerial.available() >0){
    //      Serial.println(msg);
          msg += (char)modemSerial.read();
        }
        if(msg.substring(msg.length()-2) == "OK"){
    //      Serial.println("ok");
          break;
        }
        //nie dodawac else ERROR
      }
    
    //Serial.println("---msg---");Serial.println(msg);Serial.println("---------"); 
    byte pos = msg.indexOf("8405173374F1");//newphone_nr
    //Serial.print("Pozycja numeru w wiadomosci: ");Serial.println(pos);
    
    if(pos == -1){
      //Serial.println("Zly numer");
      ;
    }
    else{
      //Serial.print("Nr telefonu: ");
      //Serial.println(msg.substring(pos,pos+12));//nr
      //String val = msg.substring(pos+34,msg.length()-21);
      //Serial.print("cmd: ");Serial.println(val);//sms
      
      if(-1 != (msg.indexOf("540065006D007"))){//Temp
        //Serial.print("cmd: Tmep - ");Serial.println(val);
        msg = "Czujnik DHT11\n";
        msg += dht.readTemperature();msg += "*C ";
        msg += dht.readHumidity();msg += "%";
        sendSms(mojNumer, msg);
      }
      else if(-1 != msg.indexOf("4200610074006")){//Bateria
        //Serial.print("cmd: Bateria - ");Serial.println(val);
        msg = sendCommand("AT+CBC", 3000);
        msg = msg.substring(5 ,msg.length()-6);//msg.indexOf("+CBC:")
        msg.concat("\nVCC: "); msg.concat(readVcc()); msg.concat("mV");
        sendSms(mojNumer, msg);
      }
      else if(-1 != msg.indexOf("5A00610073006")){//Zasieg
        //Serial.print("cmd: Zasieg - ");Serial.println(val);
        msg = sendCommand("AT+CSQ", 3000);
        msg = msg.substring(5,msg.length()-6);
        sendSms(mojNumer, msg);
      }                     //   41006B0075006D00566
      else if(-1 != msg.indexOf("41006B0075006D")){
        //Serial.print("cmd: Akumulator - ");Serial.println(val);
        msg = "11.9V demo";
        sendSms(mojNumer, msg);
      }
      else if(-1 != msg.indexOf("5300740061007")){//53007400610074003
        //Serial.print("cmd: Status - ");Serial.println(val);
        msg = sendCommand("AT+CIND?", 3000);
        msg = msg.substring(5, msg.length()-6);
        sendSms(mojNumer, msg);
      }
      else{
        //Serial.println("Nieznane polecenie...");
        msg = "Nieznane polecenie";
        sendSms(mojNumer, msg);
      }
    }
    //break;
    usunWiadomosc(c);
//    Serial.println("END"); //nie przestawiac!!!
  }
  //Serial.print("freeMemory(smsCmd/)=");Serial.println(freeMemory());
  byte heat = heating.edge();
  if (heat == 1){
    msg = "Ogrzewanie WL.\nAktualna temp. w samochodzie:  ";
    msg += dht.readTemperature();msg += "*C";
    sendSms(mojNumer, msg);
  }
  else if(heat == 0){
    msg = "Ogrzewanie WYL.\nAktualna temp. w samochodzie:  ";
    msg += dht.readTemperature();msg += "*C";
    sendSms(mojNumer, msg);
  }
}
/*
AT+CMGL - List message
AT+CMGR - Read message
AT+CMGS - Send message
AT+CMSS - Send fom memory
AT+CMGW - Write message to memory
AT+CMGD - Delete message
540065006D0070 "Temp"
540065006D00700065007200610074007500720061 - Temperatura
42006100740065007200690061 - Bateria
5A00610073006900650067 - Zasieg
41006B0075006D0075006C00610074006F0072 - Akumulator
Siłę elektromotoryczną około = 12,6 V
Napięcie znamionowe pojedynczego ogniwa 2,105 V
Minimalne napięcie (wskazujące na stan głębokiego rozładowania) - 10,8 V
Prawidłowe napięcie ładowania - minimalne: 13,9 V - maksymalne 14,5 V
Po pełnym naładowaniu napięcie szybko spada do 13,2 V a później powoli do 12,6 V
Napięcie akumulatora powinno być mierzone 12 godzin po ładowaniu aby zapewnić dokładny pomiar
Napięcie przeładowania - występujące w trakcie wydzielania wodoru > 14,4 V
53007400610074003 - Status
4F00670072007A0 - Ogrzewanie
*/
