byte buttonState = 0;         // current state of the button
byte lastButtonState = 0;     // previous state of the button
byte buttonPushCounter = 0;   // counter for the number of button presses

void StateChangeDetection(byte buttonPin) {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // przycisk wcisnięty
      buttonPushCounter++;
      //Serial.println("on");
      //Serial.print("number of button pushes:  ");
      //Serial.println(buttonPushCounter);
    } 
    else {
      // puszczenie przycisku
      //Serial.println("off");
     ; 
    }
  }
  //save the current state as the last state, 
  //for next time through the loop
  lastButtonState = buttonState;
  if(buttonPushCounter == 4)
    buttonPushCounter = 0;
}
