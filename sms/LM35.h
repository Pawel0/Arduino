void lm35Screen(){
  float tempC;
  short reading;
  lcd.home ();
  lcd.print("Czujnik LM35");
  if(currentMillis - previousMillis > interval) {
    //Serial.print("      ");
    //Serial.print(previousMillis);
    //Serial.print(" - ");
    //Serial.println(currentMillis - previousMillis);
    previousMillis = currentMillis;
    reading = analogRead(tempPin);
    tempC = reading / 9.31;
    lcd.setCursor(0,1);
    lcd.print(tempC-1);
    lcd.write(byte(0));
    lcd.print("C ");
  }
}
