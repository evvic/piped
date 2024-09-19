////////////////////////////////////////////////////////////////////////////////////////
// OVERVIEW
////////////////////////////////////////////////////////////////////////////////////////
/*
DESCRIPTION: I2C protocol demo. Using the Adafruit library. Shows how to access the IC with 
             a high level API driver, based on Adafruit demo sample.

Data Sheets
https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf


AUTHOR: Eric Brown

COMMENTS:  

HARDWARE SETUP:

Arduinio UNO              MPU-9250 IMU Breakout board

Signal Name     Pin#      Signal Name     Pin #
____________________________________________________________

Power Connections (use any +5 and GND you wish from Arduino)
VCC (5V)        ANY       V+              VCC (labeled)
GND             ANY       GND             GND (labeled)

I2C Connections from Arduino to MPU-9250
SCL             D19       SCL             SCL (labeled)
SDA             D18       SDA             SDA (labeled)

MISC

                          AD0            VCC (address line 0, set to HIGH, so address is 0x69)

Note: pullups are on breakout board, so we don't need to add them to breadboard.

HISTORY: 

*/

////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES 
////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <mpu9250.h> // cstddef & cstdint are not compatible with Uno AVR processor
#include <Wire.h>


using namespace std;

////////////////////////////////////////////////////////////////////////////////////////
// PROTOTYPES
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// DEFINES AND MACROS
////////////////////////////////////////////////////////////////////////////////////////

// I2C address of the MPU-9250 can be 0x68 or 0x69 depending on external address pin AD0
// We set AD0 pin HIGH, so the I2C address will be 0x69, else 0x68 which conflicts with another device in demos
const int MPU9250_I2C_SlaveAddress = 0x69; // temp 68

// printing mode flag, set to 0 for normal human readable, set to 1 for print for plotter via comma seperated list
#define PRINT_MODE    1

////////////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////

Mpu9250 imu(&Wire, MPU9250_I2C_SlaveAddress);

char gStringBuffer[64]; // used for printing

////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  /* Serial to display data */
  Serial.begin(115200);
  while(!Serial) {}
  Serial.write( "\n\rMPU-9250 IMU Demo using mpu9250.h Library.\n\r" );

  /* Start the I2C bus */
  Wire.begin();
  Wire.setClock(400000);

  /* Initialize and configure IMU */
  if (!imu.Begin()) {
    Serial.println("Error initializing communication with IMU");
    while(1) {}
  }
  /* Set the sample rate divider */
  if (!imu.ConfigSrd(19)) {
    Serial.println("Error configured SRD");
    while(1) {}
  }

  // give IC a moment before reading...
  delay(100);
}


//////////////////////////////////////////////////////////

void loop() 
{

  if (imu.Read()) {
    /* Display the data */
    Serial.print(imu.accel_x_mps2(), 6);
    Serial.print("\t");
    Serial.print(imu.accel_y_mps2(), 6);
    Serial.print("\t");
    Serial.print(imu.accel_z_mps2(), 6);
    Serial.print("\t");
    Serial.print(imu.gyro_x_radps(), 6);
    Serial.print("\t");
    Serial.print(imu.gyro_y_radps(), 6);
    Serial.print("\t");
    Serial.print(imu.gyro_z_radps(), 6);
    Serial.print("\t");
    Serial.print(imu.mag_x_ut(), 6);
    Serial.print("\t");
    Serial.print(imu.mag_y_ut(), 6);
    Serial.print("\t");
    Serial.print(imu.mag_z_ut(), 6);
    Serial.print("\t");
    Serial.println(imu.die_temperature_c(), 6);
  }

// delay
delay(100);
} // end loop
