#include <Arduino.h>
#include "LEDs.h"
#include "Websocket.h"
#include "WifiSetup.h"

int tCheckWebsocket = 0; // Timer to keep websocket alive

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(BUILTIN_LED,OUTPUT);

  // Initialize all pixels to 'off'
  initDisplayOff(); 

  //Connect to WiFi and Websocket Server
  connectToWifi();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - tCheckWebsocket > 1000) { // Check Websocket every second
    tCheckWebsocket = millis();
    checkWebSocketConnection(); 
    Serial.println("Stayin Alive...");
    sendWebSocketMessage("Stayin Alive..." + String(tCheckWebsocket));
  }
}

// put function definitions here:

