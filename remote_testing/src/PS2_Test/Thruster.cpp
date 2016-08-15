/*
  CSLA ROBOSUB
  Thruster class implementation 
  Contributors: Carlos Galdamez, Shian Hung
*/

#include "Thruster.h"

Thruster::Thruster(byte i, byte p):id(i),pin(p){}

void Thruster::setPWM(byte index, int x){
  pwmsInput[index] = x;
  Serial.println(toString().c_str());
}

/* Had to do this separate from the constructor. Doing it in the constructor causes the arduino to 
   not start the main loop.*/
void Thruster::initialize(){
        servo.attach(pin);
        servo.writeMicroseconds(DEADBAND);    // Initialize thruster at 1500 pwms
        delay(1000);                          // Allow 1 second for ESC to receive initilization signal, the thruster should beep at this point
}

// This will be used to display all values of thruster when ever a pwm value is changed
String Thruster::toString(){
  String comma(", ");
  return String("THRUSTER ") + id + String("PWM VALUES: ") + pwmsInput[0] + comma + pwmsInput[1] + comma + pwmsInput[2] + comma + pwmsInput[3] + comma + pwmsInput[4] + comma + pwmsInput[5];
}



