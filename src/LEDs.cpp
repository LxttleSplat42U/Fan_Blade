#include <Arduino.h>
#include "LEDs.h"
#include <SPI.h>
#include <Adafruit_DotStar.h>

#define NUMPIXELS 36 // Number of LEDs in strip
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_GBR); // GBR color order (confirm with tests)

void displayImage(int ID){
  switch(ID){
    case 0: //Turn Display Off
    {
        for(int i = 0; i < NUMPIXELS; i++) {
        strip.setPixelColor(i, 0x000000);
        }
      strip.show(); 
      break;
    }
    case 1: {   //Display image [Blue Circle]
      strip.setPixelColor(1, 0x000F00);
      strip.show(); 
      break;
    }
    default: {
      break;
    }
  }
}

void initDisplayOff(){
  strip.begin();
  strip.show(); 
}