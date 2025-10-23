/*Files used to create and setup the websocket connection and registration with the server.
All other message handling, etc is also handled/processed here using the void handleWebSocketData() function.
Images are updated and displayed using the "LED.h" and "LED.cpp" files.*/

#pragma once
#include <Arduino.h>
#include <HardwareSoftwareConfig.h>
#include "LEDs.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "WifiSetup.h"

extern int avgRPM_delay;

void initWebSocketClient();
void sendWebSocketMessage(String message);
void checkWebSocketConnection();
void handleWebSocketData(uint8_t* data, size_t len);