#pragma once
#include <HardwareSoftwareConfig.h>
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_DotStar.h>

extern int movingHIpos;
extern bool displayMovingHI;
extern int tHiscrollingtimer; // Timer for HI scrolling

void initDisplayOff();
void displayImage(int ID = -1, unsigned long colour = 0x000000, int pos = 1); // Optional parameters for colour and position
void updateLEDs(int colour = 0x000000, int movingPos = 0);