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

  Serial.print("------ STARTING SUB -----\n");
  Serial.print("------ CONFIGURING PS2 CONTROL -----\n");
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0)
    Serial.print("Found Controller, configured successful\n");
  else if(error == 1)
    Serial.print("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips\n");   
  else if(error == 2)
    Serial.print("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips\n");
  else if(error == 3)
    Serial.print("Controller refusing to enter Pressures mode, may not support it.\n");

  getControllerType(ps2x.readType());
  Serial.print("------------------------------------\n");

  Serial.print("----- INITIALIZING THRUSTERS -----\n");    
  initializeThrusters();
  Serial.print("Initialization Complete\n");
  Serial.print("Press START to enable thrusters\n");
  Serial.print("----------------------------------\n");
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
        move(0); // Forward
      else if(angle_L < -45 && angle_L > -135)
        move(1); // Backward
      else if(angle_L > -45 && angle_L < 45)
        move(3); // Right
      else
        move(2); // Left
    }
    else{
      turnOutsideOff();
    }
            
    if(ps2x.Button(PSB_PAD_UP))
      move(4);
    else if(ps2x.Button(PSB_PAD_DOWN))
      move(5);
    else
      turnInsideOff();
  }
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
    
  byte pwmsInputIndex = serialInput.substring(i1 + 1,i2).toInt();
  
  if (pwmsInputIndex < 0 || pwmsInputIndex > 5)
    return;
  
  int pwms = serialInput.substring(i2+1).toInt();
  
  if (pwms < MIN_PWM || pwms > MAX_PWM)
    return;
  
  thrusters[thrusterID - 1].setPWM(pwmsInputIndex, pwms); 
}

void turnOutsideOff(){
  for (Thruster thruster : thrusters)
    if (thruster.id % 2 == 0)
      thruster.servo.writeMicroseconds(DEADBAND);   
}

void turnInsideOff(){
  for (Thruster thruster : thrusters)
    if (thruster.id % 2 != 0)
      thruster.servo.writeMicroseconds(DEADBAND);          
}

void turnAllOff(){
  turnInsideOff();
  turnOutsideOff();
}


void move(byte pwmsIndex){
  for (Thruster thruster : thrusters)
    if (thruster.id % 2 == 0 && pwmsIndex >= 0 && pwmsIndex <= 3)                 // Control for outside thrusters
        thruster.servo.writeMicroseconds(thruster.pwmsInput[pwmsIndex]);
    else if (thruster.id % 2 != 0 && (pwmsIndex == 4 || pwmsIndex == 5))           // Control for inside thrusters
        thruster.servo.writeMicroseconds(thruster.pwmsInput[pwmsIndex]);      
}

void getControllerType(int type){
    switch(type) {
    case 0:
      Serial.print("Unknown Controller type found\n");
      break;
    case 1:
      Serial.print("DualShock Controller found\n");
      break;
    case 2:
      Serial.print("GuitarHero Controller found\n");
      break;
    case 3:
      Serial.print("Wireless Sony DualShock Controller found\n");
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
    Serial.print("THRUSTERS DISABLED\n");
    turnAllOff();
  }
  else{
    start = true;
    Serial.print("THRUSTERS ENABLED\n");
  }
}

/*
   Calls initialize method for each thruster
 */
void initializeThrusters(){
  for (Thruster t : thrusters)
    t.initialize();
}

