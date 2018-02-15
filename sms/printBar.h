void bar(int licznik, byte cod, byte cdo, byte x, byte y, byte z){
  byte c = map(licznik, cod, cdo, 5, 1);

  for(byte i=0; i<9/(c+1); i++) lcd.print(" ");
  if(c==1)for(byte i=0; i<1; i++) lcd.print(char(z));
  if(c==2)for(byte i=0; i<3; i++) lcd.print(char(z));
  if(c==3)for(byte i=0; i<5; i++) lcd.print(char(z));
  if(c==4)for(byte i=0; i<7; i++) lcd.print(char(z));
  if(c==5)for(byte i=0; i<9; i++) lcd.print(char(z));
  for(byte i=0; i<9/(c+1); i++) lcd.print(" ");
/*
   if(c==1){
     lcd.setCursor(x,y);
     lcd.print("    ");
     lcd.print(char(z));
     lcd.print("    ");
   }
   if(c==2){
     lcd.setCursor(x,y);
     lcd.print("   ");
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print("   ");
   }
   if(c==3){
     lcd.setCursor(x,y);
     lcd.print("  ");
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print("  ");
   }
   if(c==4){
     lcd.setCursor(x,y);
     lcd.print(" ");
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(" ");
   }
   if(c==5){
     lcd.setCursor(x,y);
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
     lcd.print(char(z));
   }*/
}
