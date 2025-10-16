#include <Arduino.h>
#include "HardwareSoftwareConfig.h"
#include "LEDs.h"
#include "Websocket.h"
#include "WifiSetup.h"

/*
Remember to change the ESP_ID etc in HardwareSoftwareConfig.h
%FORMAT with fan number and trailing 1
Ex, for fan "X", ID=X1
This ID is used to set the static IP address and websocket ID when connecting
*/



int tCheckWebsocket = 0; // Timer to keep websocket alive

void setup() {
  Serial.begin(115200);
  
  pinMode(BUILTIN_LED,OUTPUT);

  // Initialize all pixels to 'off'
  initDisplayOff(); 

  //Connect to WiFi and Websocket Server
  connectToWifi();
}

void loop() {
  
  if (millis() - tCheckWebsocket > 1000) { // Check Websocket every second
    tCheckWebsocket = millis();
    checkWebSocketConnection(); 
    Serial.println("Stayin Alive...");
  }

  updateLEDs(); // Update LED animations
}



