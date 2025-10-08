#include <Arduino.h>
#include "LEDs.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#define ESP_ID 21 // Unique ID of this ESP32 %FORMAT(FAN_NUMBER,1 Ex. if fan number 3 then ID=31)

void initWebSocketClient();
void sendWebSocketMessage(String message);
void checkWebSocketConnection();
void handleWebSocketData(uint8_t* data, size_t len);