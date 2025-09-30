#include <Arduino.h>
#include "LEDs.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#define ESP_ID 12 // Unique ID of this ESP32

void initWebSocketClient();
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void sendWebSocketMessage(String message);
void checkWebSocketConnection();
void handleWebSocketData(uint8_t* data, size_t len);