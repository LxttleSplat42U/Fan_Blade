#include "Websocket.h"
#include <AsyncTCP.h>

AsyncClient* tcpClient = nullptr;
const char* websocket_server = "192.168.4.1";
const int websocket_port = 80;
bool websocketConnected = false;
bool handshakeCompleted = false;

void initWebSocketClient() {
  Serial.println("Initializing WebSocket client...");
  
  // Create TCP client to connect to the server
  tcpClient = new AsyncClient();
  handshakeCompleted = false;
  websocketConnected = false;
  
  tcpClient->onConnect([](void* arg, AsyncClient* client) {
    Serial.println("TCP connected to server");
    
    // Send WebSocket handshake
    String handshake = "GET /ws HTTP/1.1\r\n";
    handshake += "Host: 192.168.4.1\r\n";
    handshake += "Upgrade: websocket\r\n";
    handshake += "Connection: Upgrade\r\n";
    handshake += "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n";
    handshake += "Sec-WebSocket-Version: 13\r\n";
    handshake += "Origin: http://192.168.4.1\r\n";
    handshake += "\r\n";
    
    client->write(handshake.c_str());
    Serial.println("WebSocket handshake sent");
  });
  
  tcpClient->onData([](void* arg, AsyncClient* client, void* data, size_t len) {
    if (!handshakeCompleted) {
      String response = String((char*)data).substring(0, len);
      Serial.println("Handshake response: " + response);
      
      if (response.indexOf("101") >= 0) {
        Serial.println("WebSocket handshake successful!");
        handshakeCompleted = true;
        websocketConnected = true;
        
        // Send initial message
        sendWebSocketMessage("ESP32 " + String(ESP_ID) + " Connected");        
      }
    } else {
      // Handle WebSocket data
      handleWebSocketData((uint8_t*)data, len);
    }
  });
  
  tcpClient->onDisconnect([](void* arg, AsyncClient* client) {
    Serial.println("TCP disconnected from server");
    websocketConnected = false;
    handshakeCompleted = false;
  });
  
  tcpClient->onError([](void* arg, AsyncClient* client, int8_t error) {
    Serial.printf("TCP connection error: %d\n", error);
    websocketConnected = false;
    handshakeCompleted = false;
  });
  
  // Connect to the server
  Serial.printf("Connecting to %s:%d\n", websocket_server, websocket_port);
  tcpClient->connect(websocket_server, websocket_port);
}

void handleWebSocketData(uint8_t* data, size_t len) {
  // Simple WebSocket frame parsing
  if (len < 2) return;
  
  uint8_t opcode = data[0] & 0x0F;
  uint8_t payloadLen = data[1] & 0x7F;
  
  if (opcode == 0x01) { // Text frame
    size_t offset = 2;
    
    // Handle extended payload length
    if (payloadLen == 126) {
      if (len < 4) return;
      payloadLen = (data[2] << 8) | data[3];
      offset = 4;
    }
    
    // Extract message
    String message = "";
    for (size_t i = 0; i < payloadLen && (offset + i) < len; i++) {
      message += (char)data[offset + i];
    }
    
    Serial.println("WebSocket message: " + message);
    
    // Handle commands
    if (message == "LED_ON" || message == "Turning ON") {
      Serial.println("Executing LED_ON");
      displayImage(1);
      sendWebSocketMessage("LED turned ON");
    } else if (message == "LED_OFF" || message == "Turning OFF") {    
      Serial.println("Executing LED_OFF");
      displayImage(0);
      sendWebSocketMessage("LED turned OFF");
      
    } else if (message == "Connected") {
      Serial.println("Server confirmed connection");
    }
  }
}

void sendWebSocketMessage(String message) {
  if (!websocketConnected || !tcpClient || !tcpClient->connected()) {
    Serial.println("WebSocket not connected - cannot send message");
    return;
  }
  
  // Create WebSocket text frame
  size_t msgLen = message.length();
  uint8_t frame[msgLen + 6];
  
  frame[0] = 0x81; // FIN + Text frame
  frame[1] = 0x80 | msgLen; // Masked + payload length
  
  // Mask (required for client-to-server)
  frame[2] = 0x12; frame[3] = 0x34; frame[4] = 0x56; frame[5] = 0x78;
  
  // Apply mask to payload
  for (size_t i = 0; i < msgLen; i++) {
    frame[6 + i] = message[i] ^ frame[2 + (i % 4)];
  }
  
  tcpClient->write((char*)frame, msgLen + 6);
  Serial.println("Sent: " + message);
}

void checkWebSocketConnection() {
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 10000) {
    lastCheck = millis();
    
    if (websocketConnected && tcpClient && tcpClient->connected()) {
      Serial.println("WebSocket: CONNECTED");
    } else {
      Serial.println("WebSocket: DISCONNECTED - attempting reconnection...");
      
      // Clean up existing connection
      if (tcpClient) {
        delete tcpClient;
        tcpClient = nullptr;
      }
      
      // Attempt reconnection
      delay(1000);
      initWebSocketClient();
    }
  }
}

// Stub functions for compatibility
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  // Not used in this simplified implementation
}