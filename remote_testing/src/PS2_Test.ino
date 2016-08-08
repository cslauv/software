/*
  CSLA ROBOSUB
  Code for remote control of thrusters
*/

#include "PS2X_lib.h"
#include "Thruster.h"

/* 
  Set pins connected to PS2 controller:
  replace pin numbers by the ones you use 
*/
   
#define PS2_DAT        5   
#define PS2_CMD        3
#define PS2_SEL        2
#define PS2_CLK        4

// Not necessary for our purposes, leave it as false

#define pressures   false
#define rumble      false

boolean start = false;

/* 
  List of thrusters, first argument is thruster id and second argument is pin for my
  current setup, however you may change the pin numbers if your setup is different 
*/
   
Thruster thrusters[8] = { Thruster(1,8),      // FRONT RIGHT HAND SIDE THRUSTER (INSIDE)
                          Thruster(2,9),      // FRONT RIGHT HAND SIDE THRUSTER (OUTSIDE)
                          Thruster(3,10),     // REAR LEFT HAND SIDE THRUSTER (INSIDE)
                          Thruster(4,12),     // REAR LEFT HAND SIDE THRUSTER (OUTSIDE)
                          Thruster(5,11),     // FRONT LEFT HAND SIDE THRUSTER (INSIDE)
                          Thruster(6,13),     // FRONT LEFT HAND SIDE THRUSTER (OUTSIDE)
                          Thruster(7,7),      // REAR RIGHT HAND SIDE THRUSTER (INSIDE)
                          Thruster(8,6)};     // REAR RIGHT HAND SIDE THRUSTER (OUTSIDE)

PS2X ps2x; // create PS2 Controller Class
int error = 0;
byte type = 0;
byte vibrate = 0;

int joystick_LX = 0;
int joystick_LY = 0;
int joystick_RX = 0;   
int joystick_RY = 0;

int angle_L = 0;
int radius_L = 0;

void setup(){
  Serial.begin(9600);
  delay(2000);  //added delay to give wireless ps2 module some time to startup, before configuring it
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0)
    Serial.println("Found Controller, configured successful \n");
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
    
  getControllerType(ps2x.readType()); 
}

void loop() {
  if(error == 1) //skip loop if no controller found
    return; 
    
  else { //DualShock Controller
    ps2x.read_gamepad(); //read controller and set large motor to spin at 'vibrate' speed

    if(ps2x.ButtonReleased(PSB_START))
        toggleOn();
    if(start == false)
      return;
    
    if (Serial.available())
      updateValues(Serial.readString()); 
      
    //add switch variable
    joystick_LX = map(ps2x.Analog(PSS_LX), 0, 256, -128, 128);
    joystick_LY = map(ps2x.Analog(PSS_LY), 0, 256, 128, -128);

    radius_L = (int) sqrt( pow( (joystick_LX), 2) + pow( (joystick_LY), 2));
    
    angle_L =  atan2(joystick_LY, joystick_LX)*180/PI;
    
    if(radius_L > 52){
      if(angle_L > 45 && angle_L < 135)
        moveForward();
      else if(angle_L < -45 && angle_L > -135)
        moveBackward();
      else if(angle_L > -45 && angle_L < 45)
        turnRight();
      else
        turnLeft(); 
    }
    else{
      turnAllOff();
    }
            
    if(ps2x.Button(PSB_PAD_UP))
      emerge();
    if(ps2x.Button(PSB_PAD_DOWN))
      submerge();    
    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
      Serial.print("Stick Values:");
      Serial.print(joystick_LY, DEC); //Left stick, Y axis. Other options: LX, RY, RX  
      Serial.print(",");
      Serial.print(joystick_LX, DEC); 
      Serial.print(",");
      Serial.print(radius_L, DEC); 
      Serial.print(",");
      Serial.println(angle_L, DEC); 
    }
  }
  delay(50);  
}

/*
  Used to update thruster power values.
  Command is sent from the pi to the arduino in the following form
  thrusterid:pwrid:pwr(1:0:0)  <--- This would mean give thruster 1 no foward thrust
*/
void updateValues(String serialInput){
  
  byte i1 = serialInput.indexOf(':');
  byte i2 = serialInput.lastIndexOf(':');
  
  if (i2 != 3)
    return;
  
  byte thrusterID = serialInput.substring(0,i1).toInt();
  
  if (thrusterID < 0 || thrusterID > 8)
    return;
    
  byte pwrIndex = serialInput.substring(i1 + 1,i2).toInt();
  
  if (pwrIndex < 0 || pwrIndex > 5)
    return;
  
  byte pwr = serialInput.substring(i2+1).toInt();
  
  if (pwr < 0 || pwr > 255)
    return;
  
  thrusters[thrusterID - 1].pwr[pwrIndex] = pwr; 
}

void turnAllOff(){
  digitalWrite(thrusters[0].pin, LOW);
  digitalWrite(thrusters[1].pin, LOW);
  digitalWrite(thrusters[2].pin, LOW);
  digitalWrite(thrusters[3].pin, LOW);
  digitalWrite(thrusters[4].pin, LOW);
  digitalWrite(thrusters[5].pin, LOW);
  digitalWrite(thrusters[6].pin, LOW);
  digitalWrite(thrusters[7].pin, LOW);
}

// Horizontal and Angular Movement Control

void moveForward(){
  analogWrite(thrusters[1].pin, thrusters[1].pwr[3]);    // Front Right Side Thruster 
  analogWrite(thrusters[3].pin, thrusters[3].pwr[3]);    // Rear Left Side Thruster
  analogWrite(thrusters[5].pin, thrusters[5].pwr[3]);    // Front Left Side Thruster
  analogWrite(thrusters[7].pin, thrusters[7].pwr[3]);    // Rear Right Side Thruster        
}

void moveBackward(){
  analogWrite(thrusters[1].pin, thrusters[1].pwr[3]);    // Front Right Side Thruster 
  analogWrite(thrusters[3].pin, thrusters[3].pwr[3]);    // Rear Left Side Thruster
  analogWrite(thrusters[5].pin, thrusters[5].pwr[3]);    // Front Left Side Thruster
  analogWrite(thrusters[7].pin, thrusters[7].pwr[3]);    // Rear Right Side Thruster  
}

void turnLeft(){
  analogWrite(thrusters[1].pin, thrusters[1].pwr[3]);    // Front Right Side Thruster 
  analogWrite(thrusters[3].pin, thrusters[3].pwr[3]);    // Rear Left Side Thruster
  analogWrite(thrusters[5].pin, thrusters[5].pwr[3]);    // Front Left Side Thruster
  analogWrite(thrusters[7].pin, thrusters[7].pwr[3]);    // Rear Right Side Thruster
}

void turnRight(){
  analogWrite(thrusters[1].pin, thrusters[1].pwr[3]);    // Front Right Side Thruster 
  analogWrite(thrusters[3].pin, thrusters[3].pwr[3]);    // Rear Left Side Thruster
  analogWrite(thrusters[5].pin, thrusters[5].pwr[3]);    // Front Left Side Thruster
  analogWrite(thrusters[7].pin, thrusters[7].pwr[3]);    // Rear Right Side Thruster
}

// Control for inside thrusters

void submerge(){
  analogWrite(thrusters[0].pin, thrusters[0].pwr[4]);    // Front Right Side Thruster
  analogWrite(thrusters[2].pin, thrusters[2].pwr[4]);    // Rear Left Side Thruster
  analogWrite(thrusters[4].pin, thrusters[4].pwr[4]);    // Front Left Side Thruster
  analogWrite(thrusters[6].pin, thrusters[6].pwr[4]);    // Rear Right Side Thruster
}

void emerge(){
  analogWrite(thrusters[0].pin, thrusters[0].pwr[5]);    // Front Right Side Thruster
  analogWrite(thrusters[2].pin, thrusters[2].pwr[5]);    // Rear Left Side Thruster
  analogWrite(thrusters[4].pin, thrusters[4].pwr[5]);    // Front Left Side Thruster
  analogWrite(thrusters[6].pin, thrusters[6].pwr[5]);    // Rear Right Side Thruster
}

void getControllerType(int type){
    switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
    case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
}

/* 
  Method used to modify boolean which enables and disables the control of
  the thrusters.
*/
void toggleOn(){
  if (start){
    start = false;
    Serial.println("THRUSTERS DISABLED");
    turnAllOff();
  }
  else{
    start = true;
    Serial.println("THRUSTERS ENABLED");
  }
}


