#ifndef GFXTEST_H
#define GFXTEST_H

#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

void testdrawtext(Adafruit_ST7735 &tft, char *text, uint16_t color);

#endif

// void testlines(uint16_t color);
// void testfastlines(uint16_t color1, uint16_t color2);

// void testdrawrects(uint16_t color);

// void testfillrects(uint16_t color1, uint16_t color2);

// void testfillcircles(uint8_t radius, uint16_t color);

// void testdrawcircles(uint8_t radius, uint16_t color);

// void testtriangles();

// void testroundrects();

// void tftPrintTest();

// void mediabuttons();