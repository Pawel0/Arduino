/*
  EdgeDetection - Library for detect change state of input pin.
  Created by , 27.01.2015r
  Released into the public domain.
*/
#ifndef EdgeDetection_h
#define EdgeDetection_h

#include "Arduino.h"

class EdgeDetection
{
  public:
    EdgeDetection(byte pin);
    byte edge();
    byte lastState();
    byte counter();
  private:
    byte _pin;
    byte _counter;
    byte _pinState;
    byte _lastPinState;
};

#endif