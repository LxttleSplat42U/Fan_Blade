#include <Arduino.h>
#include "LEDs.h"
#include <SPI.h>
#include <Adafruit_DotStar.h>

#define NUMPIXELS 36 // Number of LEDs in strip
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_GBR); // GBR color order (confirm with tests)
bool displayOff = true;
bool animated = false;
bool spiral = false;
int spiralLEDPos = 0;
int spiralAnimationSpeed = 300; // Speed of animation in milliseconds
int tspiraltimer = 0; // Timer for spiral animation
int currentPosition = 0;
int currentPattern = 0;

void displayImage(int ID){
  switch(ID){
    case 0: {   //Display image Blue Circle
      displayOff = false;
      strip.clear();
      strip.setPixelColor(0, 0x000F00);
      strip.show(); 
      spiral = false;
      break;
    }
    case 1: {   //Display image Spiral
      displayOff = false;
      strip.clear();
      strip.setPixelColor(0, 0x00000F);
      strip.show(); 
      spiral = true;
      spiralLEDPos = 0;
      break;
    }
    default: { //Turn off display [-1]
      displayOff = true;
      strip.clear();
      strip.show(); 
      break;
    }
  }
}

void updateLEDs(){
  if (displayOff) return; //Do nothing if display is off

  if (spiral && (tspiraltimer - millis() > spiralAnimationSpeed)) {
    tspiraltimer = millis();
    strip.setPixelColor(spiralLEDPos, 0x000000); //Turn current LED off
    spiralLEDPos += 1;
    if (spiralLEDPos >= NUMPIXELS) {
      spiralLEDPos = 0; //Loop back to start
    }
    strip.setPixelColor(spiralLEDPos, 0x00000F); //Turn next LED on
    strip.show();
  }
}

void initDisplayOff(){
  strip.begin();
  strip.show(); 
}