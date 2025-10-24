#pragma once
#include <Arduino.h>
#include <array>


extern const int NUMPIXELS; // Number of LEDs in strip
extern const int ESP_ID ; // Unique ID of this ESP32
extern const char* ssid; //Main ESP32 Access Point SSID
extern const char* password; //Password if required
extern const int letterE[36][36];
extern const int letterMNS[36][36];
extern const int letterHI[36][36];
extern const std::array<float, 36> delta_t; // Time delay array for LED positions
