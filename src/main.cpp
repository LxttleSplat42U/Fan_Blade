#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_DotStar.h>

int OnBoardLED = 21;
#define NUMPIXELS 36 // Number of LEDs in strip

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);

void setup() {
  // put your setup code here, to run once:
  pinMode(OnBoardLED,OUTPUT);

  // Initialize all pixels to 'off'
  strip.begin();
  strip.show(); 
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(OnBoardLED,HIGH);
  delay(1000);
  digitalWrite(OnBoardLED,LOW);
  delay(1000);
  strip.setPixelColor(1, 0xFF0000); // Blue
  strip.show(); 
}

// put function definitions here:
