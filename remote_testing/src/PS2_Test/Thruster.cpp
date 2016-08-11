/*
  CSLA ROBOSUB
  Thruster class implementation 
  Contributors: Carlos Galdamez, Shian Hung
*/

#include "Thruster.h"

Thruster::Thruster(byte i, byte p):id(i),pin(p){
    //pinMode(pin, OUTPUT);    // Set pin as output pin
    servo.attach(pin,MIN_PWM,MAX_PWM);
}

void Thruster::setPWM(byte index, int x){
  pwmsInput[index] = x;
}
