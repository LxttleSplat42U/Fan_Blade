#include <LEDs.h>

#define NUMPIXELS 36 // Number of LEDs in strip
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BGR); // (confirm with tests as DotStars differ in color order)
bool displayOff = true;
bool animated = false;
bool spiral = false;
int spiralLEDPos = 0;
int spiralAnimationSpeed = 100; // Speed of animation in milliseconds
int tspiraltimer = 0; // Timer for spiral animation
int currentPosition = 0;
int currentPattern = 0;

void displayImage(int ID, unsigned long colour, int pos){

  // Reset display
  strip.clear();
  displayOff = false;
  spiral = false;

  switch(ID){
    case 0: {   //Display image Circle      
      strip.setPixelColor(0, 0x000F00);
      strip.show();       
      break;
    }
    case 1: {   //Display image Spiral
      strip.setPixelColor(0, 0x00000F);
      strip.show(); 
      spiral = true;
      spiralLEDPos = 0;
      break;
    }
    case 2:{
      strip.setPixelColor(pos, colour);
      strip.show(); 
      break;
    }

    default: { //Turn off display [-1]
      displayOff = true;
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
  strip.clear();
  strip.show(); 
}