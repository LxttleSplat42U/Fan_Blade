#pragma once
#include <HardwareSoftwareConfig.h>
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_DotStar.h>

void initDisplayOff();
void displayImage(int ID = -1, unsigned long colour = 0x000000, int pos = 1); // Optional parameters for colour and position
void updateLEDs(int colour = 0x000000);