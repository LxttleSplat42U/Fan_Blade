#include "Websocket.h"

AsyncClient* tcpClient = nullptr;
const char* websocket_server = "192.168.4.1"; //Change if different address
const int websocket_port = 80;                //Change if different port
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
    Serial.printf("Received %d bytes of data\n", len);
    
    if (!handshakeCompleted) {
      String response = String((char*)data).substring(0, len);
      Serial.println("Handshake response: " + response);
      
      if (response.indexOf("101") >= 0) {
        Serial.println("WebSocket handshake successful!");
        handshakeCompleted = true;
        websocketConnected = true;
        
        // Send initial to register as client with a unique ID
        sendWebSocketMessage("REGISTER:" + String(ESP_ID));   
        Serial.println("Sent registration message: " + String(ESP_ID));     
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
  Serial.println("New Data");
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
    else if (payloadLen == 127) {
      // Handle 64-bit length (simplified)
      if (len < 10) return;
      payloadLen = data[9]; // Use only lower byte for simplicity
      offset = 10;
    }
    
    // Check if we have enough data for the complete payload
    if (len < offset + payloadLen) {
      Serial.println("Incomplete WebSocket frame received");
      return;
    }

    // Extract message
    String message = "";
    for (size_t i = 0; i < payloadLen && (offset + i) < len; i++) {
      message += (char)data[offset + i];
    }
    
    Serial.println("WebSocket message: " + message);

    //Check if shutdown command
    if (message == "-1") {
      displayImage(-1); //Turn off display
      Serial.println("Received shutdown command - turning off display");
      return;
    }

    //Split message into parts
    int firstColon = message.indexOf(':');
    int secondColon = message.indexOf(':', firstColon + 1);
    int thirdColon = message.indexOf(':', secondColon + 1);

     // Check if we have a valid format
    if (firstColon == -1 || secondColon == -1) {
      Serial.println("Invalid message format - expected TARGET:COMMAND:VALUE");
      Serial.println("Received: " + message);
      return;
    } 

    String targetClient = message.substring(0, firstColon);
    String command = message.substring(firstColon + 1, secondColon);
    String value = message.substring(secondColon + 1, thirdColon);

    // Debug output
    Serial.println("Target: " + targetClient);
    Serial.println("Command: " + command);
    Serial.println("Value: " + value);

    // Handle commands
    if (targetClient == (String(ESP_ID))) {
      if (command == "DISPLAY"){

        if (value == "2") { //Custom Circle
        int fourthColon = message.indexOf(':', thirdColon + 1);
        int fifthColon = message.indexOf(':', fourthColon + 1);
        String colour = message.substring(thirdColon + 1, fourthColon);
        String posLED = message.substring(fourthColon + 1, fifthColon);

        // Convert hex string colour to (Ex, "0x0000FF") to long integer
        unsigned long colourHEX = strtol(colour.c_str(), nullptr, 16);
        //displayImage(2, colourValue, posLED.toInt());
        // Serial.println("Setting display image to: " + String(value.toInt()) + " Colour: " + String(colourHEX, HEX) + " Position: " + posLED);
        displayImage(2, colourHEX, posLED.toInt());
        }
        else{
          displayImage(value.toInt());
          // Serial.println("Setting display image to: " + String(value.toInt()));
        }

        
      }
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
      displayImage(-1); // Turn off display on disconnect
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