void sr04Screen(){
  short previousMillis2 = 0;
  short duration, distance;
  lcd.home ();
  lcd.print("Czujnik HC-SR04");
  if(currentMillis - previousMillis > interval/2) {
  // save the last time you blinked the LED 
    previousMillis = currentMillis;
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH, 30000);
    distance = (duration/2) / 29.1;
    if (distance < 2 && duration >= 58) {
      //Serial.println(duration);
      lcd.setCursor(0,1);
      lcd.print("<002");
    }
    else if(duration < 58 || distance > 370){
      lcd.setCursor(0,1);
      lcd.print(">370");
    }
    else {
      //Serial.print(distance);
      //Serial.println("cm");
      //Serial.println(duration);
      lcd.setCursor(0,1);
      lcd.print(" ");
      if(distance<10){
        lcd.print("00");
        lcd.setCursor(3,1);
      }
      if(distance>9 && distance<100){
        lcd.print("0");
        lcd.setCursor(2,1);
      }
      lcd.print(distance);
      lcd.print("cm");
    }
    bar(distance, 1, 30, 7, 1, 255);
  }
  if(currentMillis - previousMillis > map(distance, 2, 100 ,50, 500)) {
    previousMillis2 = currentMillis; 
    tone(9, 1100, 20);
  }
}
