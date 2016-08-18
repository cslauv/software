/*
* CSLAUV ROBOSUB
* contributors: Carlos Galdamez
*/

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>

Adafruit_LSM9DS0 imu = Adafruit_LSM9DS0();

/* 
 * The values that the sensors are being configured are the smallest possible values.
 * When the values are higher the sensors can measure larger movements but it less precise.
 */
void configureSensors(){
  imu.setupAccel(imu.LSM9DS0_ACCELRANGE_2G);
  Serial.print("Accelerometer Configured Successfully\n");
  imu.setupMag(imu.LSM9DS0_MAGGAIN_2GAUSS);
  Serial.print("Magnetometer Configured Successfully\n");
  imu.setupGyro(imu.LSM9DS0_GYROSCALE_245DPS);
  // imu.setupGyro(imu.LSM9DS0_GYROSCALE_500DPS);
  // imu.setupGyro(imu.LSM9DS0_GYROSCALE_2000DPS);
  Serial.print("Gyro Configured Successfully\n");
}


void setup(){
  Serial.begin(9600);
  Serial.print("----- SETTING UP -----\n");
  
  /* Initialize the sensor */
  if(!imu.begin())
  {
    /* There was a problem detecting the LSM9DS0 ... check your connections */
    Serial.print("No LSM9DS0 detected ... Check your wiring or I2C ADDR!\n");
    while(1);   // Blocking loop
  }

  Serial.print("----- CONFIGURING SENSORS -----\n");
  configureSensors();
  Serial.print("-------------------------------\n");
}

void loop(){
  sensors_event_t accel, mag, gyro, temp;
  imu.getEvent(&accel, &mag, &gyro, &temp);
  Serial.print("ACCELEROMETER DATA: ");
  Serial.print(accel.acceleration.x); Serial.print(", ");
  Serial.print(accel.acceleration.y); Serial.print(", ");
  Serial.print(accel.acceleration.z); Serial.print("\n\n");
  Serial.print("GYRO DATA: ");
  Serial.print(gyro.gyro.x); Serial.print(", ");
  Serial.print(gyro.gyro.y); Serial.print(", ");
  Serial.print(gyro.gyro.z); Serial.print("\n\n");
  Serial.print("MAGNETOMETER DATA: ");
  Serial.print(mag.magnetic.x); Serial.print(", ");
  Serial.print(mag.magnetic.y); Serial.print(", ");
  Serial.print(mag.magnetic.z); Serial.print("\n");

  delay(1000);  // Will provide new data every second
}


