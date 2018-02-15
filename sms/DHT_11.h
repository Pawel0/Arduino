void dhtScreen(){
  lcd.home();
  lcd.print("Czujnik DHT11");
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    int h = dht.readHumidity();
    int t = dht.readTemperature();// Read temperature as Celsius
    //float f = dht.readTemperature(true);// Read temperature as Fahrenheit
    if (isnan(h) || isnan(t)) {//if (isnan(h) || isnan(t) || isnan(f)) {
//      Serial.println("Failed to read from DHT sensor!");
    }
    lcd.setCursor(0,1);
    lcd.print(t);
    lcd.print(char(0));
    lcd.print("C");
    lcd.setCursor(10,1);
    lcd.print(h);
    lcd.print("%");
  }
}
