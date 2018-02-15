void mvccScreen(){
  lcd.home();
  lcd.print("Napiecie zasila.");
  if(currentMillis - previousMillis > interval/2) {
    previousMillis = currentMillis;
    lcd.setCursor(0,1);
    lcd.print(readVcc());
    lcd.print("mV");
  }
}
