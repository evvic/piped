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
int iterations = 0;

// For 1.44" and 1.8" TFT with ST7735:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////

void drawtext(Adafruit_ST7735 &tft, char *text, uint16_t color, uint8_t size = 1, int x = 0, int y = 0) {
  tft.setCursor(x, y);
  tft.setTextSize(size);
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
  tft.fillRect(x, 0, w, pipe_height, ST77XX_GREEN);
  tft.fillRoundRect(x - offset, pipe_height, w2, pipe_height/2, 2, ST77XX_GREEN);
}

void drawClosingPipe(int x, int w, int pipe_height = 5) {
  int PIPE_OPENING_DIFFERENCE_PX = 6;
  if (x < 0 || x > tft.width()) x = 0;

  int w2 = w + PIPE_OPENING_DIFFERENCE_PX; // w2 SHOULD be larger than w1 (the pipe should have a larger opening part)
  int offset = (w2/2) - (w/2);
  tft.fillRect(x, tft.height() - pipe_height, w, pipe_height, ST77XX_GREEN);
  tft.fillRoundRect(x - offset, tft.height() - pipe_height - (pipe_height/2), w2, pipe_height/2, 2, ST77XX_GREEN);
}

int generateClosingPipeXpos(int screen_width, int pipe_width) {
  return random(0, screen_width - pipe_width);
}

void writeScore(int score) {
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(4);
}

int gameplayLoop(int pipe_width, int score) {
  int player_height = 0;
  int frame;

  int closing_xpos = generateClosingPipeXpos(tft.width(), pipe_width);
  int starting_player_xpos = player_pos - (pipe_width/2);

  // sprintf(stringBuffer, "Score: %d", score); // this is too long
  sprintf(stringBuffer, "%d", score);

  drawOpeningPipe(starting_player_xpos, pipe_width);
  drawClosingPipe(closing_xpos, pipe_width);

  while (player_height < tft.height() - RADIUS) {
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
    // 
    // Serial.println(stringBuffer);

    if (player_height < RADIUS*3) {
      drawOpeningPipe(starting_player_xpos, pipe_width);
      // write score
      drawtext(tft, stringBuffer, ST77XX_WHITE, 1);
    }
    else if (player_height > tft.height() - RADIUS*3) {
      drawClosingPipe(closing_xpos, pipe_width);
    }

    delay(100);
  }

  // determine if player won the round
  if (player_pos - RADIUS < closing_xpos || player_pos + RADIUS > closing_xpos + pipe_width) return 1;

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
  drawtext(tft, "Warming up...", ST77XX_WHITE);
  delay(1000);

  tft.fillScreen(ST77XX_BLUE);
  iterations = 0;
}

void loop() {
  // put your main code here, to run repeatedly:

  // DO NOT LET pipe_width GET SMALLER THAN RADIUS*2
  int pipe_width = RADIUS * 5 - (int)sqrt(iterations*20.0);
  if (pipe_width <= RADIUS*2) pipe_width = RADIUS*2;

  tft.fillScreen(ST77XX_BLUE);
  int result = gameplayLoop(pipe_width, iterations);
  
  // Game Over screen
  if (result != 0) {
    tft.fillScreen(ST77XX_RED);
    drawtext(tft, "GAME OVER", ST77XX_WHITE, 4, 10, 5);
    sprintf(stringBuffer, "Score: %d", iterations);
    drawtext(tft, stringBuffer, ST77XX_WHITE, 2, 10, 70);
    iterations = 0;
    
    delay(10000);
  }
  else iterations++;

}


