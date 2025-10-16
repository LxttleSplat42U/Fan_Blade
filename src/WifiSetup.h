#pragma once
#include <HardwareSoftwareConfig.h>
#include <Websocket.h>
#include <WiFi.h>
#include <Arduino.h>

// Unique ID of this ESP32 %FORMAT with fan number and trailing 1
// Ex, for fan "X", ID=X1
//This ID is used to set the static IP address and websocket ID when
//registering
  
void connectToWifi();