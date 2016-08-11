/*
  CSLA ROBOSUB
  Thruster class header file 
  Contributors: Carlos Galdamez, Shian Hung
*/

#ifndef Thruster_H
#define Thruster_H

#include <Arduino.h>
#include <Servo.h>

#define MIN_PWM     1100
#define MAX_PWM     1900
#define DEADBAND    1500

class Thruster{
  public:
    byte id;      // Thruster ID, number written on robosub prototype
    byte pin;     // Pin which thruster is connected to
    Servo servo;
    Thruster(byte id, byte pin);
    void setPWM(byte index, int x);
    // All thrusters start with max power in the beginning
    ///byte pwr[6] = {255,255,255,255,255,255};    // {forward,backwards,left,right,submerge,emerge}
    
    /* This is the one we will actually use for the thrusters, the pulse width signal input will range from
    1100 to 1900. 1500 is  */
    int pwmsInput[6] = {DEADBAND,DEADBAND,DEADBAND,DEADBAND,DEADBAND,DEADBAND};    // {forward,backwards,left,right,submerge,emerge}

};

#endif
