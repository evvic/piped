#include <SPI.h>
#include <TFT.h>
// #include <Adafruit_GFX.h>    // Core graphics library
// #include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

// #define PIN_SD_CS 4
#define PIN_TFT_CS 10
#define PIN_DC 9
#define PIN_RST 8

#define DELAY_IMAGE_SWAP 60000 // each image is shown for 60 seconds

TFT TFTscreen = TFT(PIN_TFT_CS, PIN_DC, PIN_RST);
// Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
// Adafruit_ST7735 tft = Adafruit_ST7735(PIN_TFT_CS, PIN_DC, PIN_RST);

int new_color;
/*
void setup() {
  // initialize default serial connection to send debug information
  Serial.begin(9600);
  while (!Serial) {
    // wait until default serial connection is fully set up
  }
  
  //The following two lines replace "TFTscreen.begin();" => avoids that red and blue (?) are swapped/interchanged
  TFTscreen.initR(INITR_BLACKTAB);
  TFTscreen.setRotation(2);

  new_color = 0;
  
  
  TFTscreen.background(255, 255, 255); // prints black screen to TFT display

  delay(1000);
  
  TFTscreen.background(255, 0, 0); // prints black screen to TFT display

  delay(1000);
  
  TFTscreen.background(0, 255, 0); // prints black screen to TFT display
}




void loop() {

  TFTscreen.background(new_color % 255, 255, 255 - (new_color % 255)); // prints black screen to TFT display

  // set the text color to white
  TFTscreen.stroke(0,0,0);

  TFTscreen.setTextSize(2);
  // write text to the screen in the top left corner
  TFTscreen.text("Testing!", 5, 5);

  new_color+=15;
  
  delay(500);
}

*/


