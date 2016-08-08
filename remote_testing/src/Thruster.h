/*
  CSLA ROBOSUB
  Thruster class header file 
  Contributors: Carlos Galdamez, Shian Hung
*/

#ifndef Thruster_H
#define Thruster_H

#include <Arduino.h>

class Thruster{
  public:
    byte id;      // Thruster ID, number written on robosub prototype
    byte pin;     // Pin which thruster is connected to
    Thruster(byte id, byte pin);
    void setPwr(byte index, byte x);
    // All thrusters start with max power in the beginning
    byte pwr[6] = {255,255,255,255,255,255};    // {forward,backwards,left,right,submerge,emerge}
};

#endif
