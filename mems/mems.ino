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

#include <mpu9250.h>
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
const int MPU9250_I2C_SlaveAddress = 0x68; // temp 68

// printing mode flag, set to 0 for normal human readable, set to 1 for print for plotter via comma seperated list
#define PRINT_MODE    1

////////////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////

MPU9250 imu(Wire, MPU9250_I2C_SlaveAddress);

char gStringBuffer[64]; // used for printing

////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////

void setup() 
{
// initialize serial port
Serial.begin(115200);
Serial.write( "\n\rMPU-9250 IMU Demo using mpu9250.h Library.\n\r" );

// slow down I2C clock
Wire.setClock( 100000 );

// initialize the MPU if found on bus
if (!mpu.begin(MPU9250_I2C_SlaveAddress)) 
  {
  while (1) 
    {
    Serial.println("Failed to find MPU-9250 chip");
    delay(100);
    } // end while
  } // end if



// give IC a moment before reading...
delay(100);  

} // end setup

//////////////////////////////////////////////////////////

void loop() 
{

// delay
delay(100);
} // end loop
