/*
  CSLA ROBOSUB
  Thruster class implementation 
  Contributors: Carlos Galdamez, Shian Hung
*/

#include "Thruster.h"

Thruster::Thruster(byte i, byte p):id(i),pin(p){
    pinMode(pin, OUTPUT);    // Set pin as output pin
}

void Thruster::setPwr(byte index, byte x){
  pwr[index] = x;
}
