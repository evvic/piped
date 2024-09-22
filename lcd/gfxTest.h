#ifndef GFXTEST_H
#define GFXTEST_H

#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

void testdrawtext(Adafruit_ST7735 &tft, char *text, uint16_t color);
void testlines(Adafruit_ST7735 &tft,uint16_t color);
void testfastlines(Adafruit_ST7735 &tft, uint16_t color1, uint16_t color2);
void testdrawrects(Adafruit_ST7735 &tft, uint16_t color);
void testfillrects(Adafruit_ST7735 &tft, uint16_t color1, uint16_t color2);
void testfillcircles(Adafruit_ST7735 &tft, uint8_t radius, uint16_t color);
void testdrawcircles(Adafruit_ST7735 &tft, uint8_t radius, uint16_t color);
void testtriangles(Adafruit_ST7735 &tft);
void testroundrects(Adafruit_ST7735 &tft);
void tftPrintTest(Adafruit_ST7735 &tft, float p = 3.1415926);
void mediabuttons(Adafruit_ST7735 &tft);

#endif
