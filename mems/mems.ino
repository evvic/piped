////////////////////////////////////////////////////////////////////////////////////////
// OVERVIEW
////////////////////////////////////////////////////////////////////////////////////////
/*
DESCRIPTION: I2C protocol demo. Using the Wire.h library only, directly reads from the 
             MPU-9250 IMU. Shows how to access the IC without a high level API driver.

Data Sheets
https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-9250-Datasheet1.pdf
https://invensense.tdk.com/wp-content/uploads/2015/02/RM-MPU-9250A-00-v1.6.pdf


AUTHOR: Eric Brown

COMMENTS:  

HARDWARE SETUP:

Arduinio UNO              MPU-9250 IMU Breakout board - Elegoo Most Complete Arduino Uno R3 Kit version
   

Signal Name     Pin#      Signal Name     Pin #
____________________________________________________________

Power Connections (use any +5 and GND you wish from Arduino)
VCC (5V)        ANY       V+              VCC (labeled)
GND             ANY       GND             GND (labeled)

I2C Connections from Arduino to MPU-9250
SCL             D19       SCL             SCL (labeled)
SDA             D18       SDA             SDA (labeled)

MISC

                          AD0             Ground (address line 0)

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

#include "Wire.h"  // I2C / 2-wire library

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////
// PROTOTYPES
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// DEFINES AND MACROS
////////////////////////////////////////////////////////////////////////////////////////

// I2C address of the MPU-9250 can be 0x68 or 0x69 depending on external address pin AD0
// We set AD0 pin HIGH, so the I2C address will be 0x69, else 0x68 which conflicts with another device in demos
const int MPU9250_I2C_SlaveAddress = 0x68; 

////////////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////

char gStringBuffer[64]; // used for printing

int16_t accel_x, accel_y, accel_z; // accelerometer raw data (16-bit, 2's complement)
int16_t gyro_x, gyro_y, gyro_z;    // gyro raw data  (16-bit, 2's complement)
int16_t temp;                      // temperature sensor data  (16-bit, 2's complement)
uint8_t device_id;

int screen_width = 100;
char insert_char = 'X';
int insert_index = 50; // Adjust the index as needed
char fillChar = ' ';
////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////

void printPosition(int insert_index, int screen_width = 100, char insert_char = 'X') {
  if (insert_index > screen_width) insert_index = screen_width;
  else if (insert_index < 0) insert_index = 0;

  for (int i = 0; i < insert_index; i++) Serial.print(" ");

  Serial.print(insert_char);

  Serial.println(insert_index);

// TEMP serial
// Create a String of spaces
// String output(screen_width, fillChar);
// String firstPart = output.substring(0, insert_index);
// String secondPart = output.substring(insert_index + 1);
// output = firstPart + insertChar + secondPart;
// Serial.println(output);

}

void setup() 
{
// initialize serial port
Serial.begin(115200);
Serial.write( "\n\rMPU-9250 IMU Demo.\n\r" );


// call the begin method with or without the slave address
Wire.begin();  // if you want to change something and restart lib, call Wire.end();

// set power management and wake up, bare minimum, ignore the countless other settings, we just want to 
// turn the IC on and get some raw data
Wire.beginTransmission( MPU9250_I2C_SlaveAddress ); // Begins a transmission to the I2C slave (GY-521 board)

Wire.write( 0x6B ); // PWR_MGMT_1 register
Wire.write( 0x00 ); // reset the IC and set registers to defaults 

// end transaction
Wire.endTransmission(true);
} // end setup

//////////////////////////////////////////////////////////

void loop() 
{
// begin transaction, address the device  
Wire.beginTransmission( MPU9250_I2C_SlaveAddress );

// we are going read a block of bytes starting at address 0x3B (ACCEL_XOUT_H)
// which is begins the block containing the accelerometer, gyro, and temp
// See MPU-6000 and MPU-9250 Register Map and Descriptions p.40
Wire.write( 0x3B ); 

// end transaction, but the FALSE indicates that a STOP condition is not set, so the bus stays active
// waiting for a "Repeated Start", which is what we are going to do next, but with a READ operation
Wire.endTransmission(false); 

// this causes a repeated START condition and allows is to read bytes
// we need to read 7 registers, each 2 bytes, so 14 bytes
Wire.requestFrom(MPU9250_I2C_SlaveAddress, 14, true); 

// read the high and low bytes (big endian) for accelerometer
accel_x = Wire.read()<<8 | Wire.read(); // read registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
accel_y = Wire.read()<<8 | Wire.read(); // read registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
accel_z = Wire.read()<<8 | Wire.read(); // read registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)

// read the high and low bytes (big endian) for temp
temp = Wire.read()<<8 | Wire.read(); // read registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)

// read the high and low bytes (big endian) for gyro
gyro_x = Wire.read()<<8 | Wire.read(); // read registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
gyro_y = Wire.read()<<8 | Wire.read(); // read registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
gyro_z = Wire.read()<<8 | Wire.read(); // read registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

// print out accel data
Serial.print("Accel(");    
Serial.print( (float)accel_x/16384.0f ); Serial.print(",");    
Serial.print( (float)accel_y/16384.0f ); Serial.print(",");    
Serial.print( (float)accel_z/16384.0f ); Serial.print(")");    
Serial.print(" | ");

// print out gyro data
Serial.print("Gyro(");    
Serial.print( (float)gyro_x/16384.0f ); Serial.print(",");    
Serial.print( (float)gyro_y/16384.0f ); Serial.print(",");    
Serial.print( (float)gyro_z/16384.0f ); Serial.print(")");    
Serial.print(" | ");

insert_index += (int)(accel_y/16384.0f * 10);
if (insert_index > screen_width) insert_index = screen_width;
else if (insert_index < 0) insert_index = 0;

// printPosition(insert_index);


// temperature equation in C (celcius) was taken from the datasheet (MPU-6000/MPU-9250 Register Map and Description, p.30)
// then modified to convert to F, i.e. multiply by 1.8 and add 32.0
// Serial.print("temp = "); Serial.print(((float)(temp)/340.00f+36.53f)*1.8f+ 32.0f);

// begin transaction, address the device  
Wire.beginTransmission( MPU9250_I2C_SlaveAddress );

// we are going read a block of bytes starting at address 0x3B (ACCEL_XOUT_H)
// which is begins the block containing the accelerometer, gyro, and temp
// See MPU-6000 and MPU-9250 Register Map and Descriptions p.40
Wire.write( 0x75 ); 

// end transaction, but the FALSE indicates that a STOP condition is not set, so the bus stays active
// waiting for a "Repeated Start", which is what we are going to do next, but with a READ operation
Wire.endTransmission(false); 

// this causes a repeated START condition and allows is to read bytes
// we need to read 1 register, of 1 byte, so 1 byte
Wire.requestFrom(MPU9250_I2C_SlaveAddress, 1, true);

device_id = Wire.read();
Serial.print(" | id = 0x"); Serial.print(device_id, HEX);

Serial.println();

// delay
delay(100);
} // end loop


