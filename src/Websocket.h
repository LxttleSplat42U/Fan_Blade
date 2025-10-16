#pragma once
#include <Arduino.h>
#include <HardwareSoftwareConfig.h>
#include "LEDs.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "WifiSetup.h"

void initWebSocketClient();
void sendWebSocketMessage(String message);
void checkWebSocketConnection();
void handleWebSocketData(uint8_t* data, size_t len);