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
#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

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

// These pins will also work for the 1.8" TFT shield.
#define TFT_CS        10
#define TFT_RST        8 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         9

////////////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////

char stringBuffer[64]; // used for printing

int16_t accel_x, accel_y, accel_z; // accelerometer raw data (16-bit, 2's complement)
int16_t gyro_x, gyro_y, gyro_z;    // gyro raw data  (16-bit, 2's complement)
int16_t temp;                      // temperature sensor data  (16-bit, 2's complement)
uint8_t device_id;

int RADIUS = 10;
int screen_width = 100;
char insert_char = 'X';
int player_pos = 50; // Adjust the index as needed
char fillChar = ' ';

// For 1.44" and 1.8" TFT with ST7735:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////

void testdrawtext(Adafruit_ST7735 &tft, char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

int accelerateHeight(int height, int frame = 0, int gravity = 9) {
  return (height + 1 * gravity * frame * frame);
}

void drawOpeningPipe(int x, int w, int pipe_height = 5) {
  int PIPE_OPENING_DIFFERENCE_PX = 6;
  if (x < 0 || x > tft.width()) x = 0;

  int w2 = w + PIPE_OPENING_DIFFERENCE_PX; // w2 SHOULD be larger than w1 (the pipe should have a larger opening part)
  int offset = (w2/2) - (w/2);
  tft.fillRect(x + offset, 0, w, pipe_height, ST77XX_GREEN);
  tft.fillRoundRect(x, pipe_height, w2, pipe_height/2, 2, ST77XX_GREEN);
}

void drawClosingPipe(int x, int w, int pipe_height = 5) {
  int PIPE_OPENING_DIFFERENCE_PX = 6;
  if (x < 0 || x > tft.width()) x = 0;

  int w2 = w + PIPE_OPENING_DIFFERENCE_PX; // w2 SHOULD be larger than w1 (the pipe should have a larger opening part)
  int offset = (w2/2) - (w/2);
  tft.fillRect(x + offset, tft.height() - pipe_height, w, pipe_height, ST77XX_GREEN);
  tft.fillRoundRect(x, tft.height() - pipe_height - (pipe_height/2), w2, pipe_height/2, 2, ST77XX_GREEN);
}

int gameplayLoop() {
  int player_height = 0;
  int frame;

  

  drawOpeningPipe(20, RADIUS * 2 + 2);
  drawClosingPipe(25, RADIUS * 2 + 2);

  while (player_height < tft.height()) {
    // begin transaction, address the device  
    Wire.beginTransmission( MPU9250_I2C_SlaveAddress );
    Wire.write( 0x3D );
    Wire.endTransmission(false); 
    Wire.requestFrom(MPU9250_I2C_SlaveAddress, 2, true); 
    accel_y = Wire.read()<<8 | Wire.read(); // read registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)

    // tft.fillCircle(player_pos, 15, 10, ST77XX_BLUE);

    player_pos += (int)(accel_y/16384.0f * 15);
    if (player_pos > tft.width()) player_pos = tft.width();
    else if (player_pos < 0) player_pos = 0;

    // tft.fillScreen(ST77XX_BLUE);
    // tft.drawLine(player_pos, tft.height() - 10, player_pos, tft.height() - 20, ST77XX_YELLOW);
    tft.fillCircle(player_pos, player_height, RADIUS, ST77XX_YELLOW);

    player_height += 5;

    // player_height = accelerateHeight(player_height, frame++);
    sprintf(stringBuffer, "Height: %d, frame: %d", player_height, frame);
    Serial.println(stringBuffer);

    delay(100);
  }

  return 0;
  
}

void setup() {
  // call the begin method with or without the slave address
  Wire.begin();  // if you want to change something and restart lib, call Wire.end();

  // set power management and wake up, bare minimum, ignore the countless other settings, we just want to 
  // turn the IC on and get some raw data
  Wire.beginTransmission( MPU9250_I2C_SlaveAddress ); // Begins a transmission to the I2C slave (GY-521 board)

  Wire.write( 0x6B ); // PWR_MGMT_1 register
  Wire.write( 0x00 ); // reset the IC and set registers to defaults 

  // end transaction
  Wire.endTransmission(true);

  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

  tft.setRotation(2);

  // large block of text
  tft.fillScreen(ST77XX_BLACK);
  testdrawtext(tft, "Hello World", ST77XX_WHITE);
  tft.fillScreen(ST77XX_BLUE);

}

void loop() {
  // put your main code here, to run repeatedly:

  tft.fillScreen(ST77XX_BLUE);
  gameplayLoop();

}


