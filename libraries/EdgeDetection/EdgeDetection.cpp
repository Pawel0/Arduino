/*
  EdgeDetection.cpp - Library for detect change state of imput pin.
  Created by , 27.01.2015r
  Released into the public domain.
*/

#include "Arduino.h"
#include "EdgeDetection.h"

EdgeDetection::EdgeDetection(byte pin)
{
  _pin = pin;
}

byte EdgeDetection::edge()
{
  _pinState = digitalRead(_pin);
  if (_pinState != _lastPinState) {
    if (_pinState == HIGH) {
      //zmiana stanu z LOW na HIGH
      _lastPinState = _pinState;
      return 1;
    } 
    else {
      //ziana stanu z HIGH na LOW
      _lastPinState = _pinState;
      return 0;
    }
  }
  return 2;
}

byte EdgeDetection::lastState()
{
  return _lastPinState;
}

byte EdgeDetection::counter()
{
  return _counter;
}