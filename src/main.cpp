#include <Arduino.h>

#include <WiFi.h> //Used to enable WiFi functionality
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

const int FAN_ID = 1; //Set fan ID
const int Monitor_LED = D13;
bool Monitor_LED_State;
int tblink = 0; 

//WiFi Name and Password
const char* ssid = "Holo3D"; 
AsyncWebServer server(80); //Create a server on port 80
IPAddress staticIP(192, 168, 4, 1); // IP address of the ESP32 
IPAddress gateway(192, 168, 4, 1);  // Same as local_IP for AP 
IPAddress subnet(255, 255, 255, 0); // Subnet mask

//Setup WebSocket
AsyncWebSocket ws("/ws"); //Create a WebSocket object

// put function declarations here:
void initWiFiAP(); //Initialize WiFi
void initWebSocket(); //Initialize WebSocket
void LED_Blink(); //Used to check if board is reponsive/active by monitoriung the built in D13 LED flashing
void onWebEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len); //WebSocket Event

void setup() {
  // put your setup code here, to run once:
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/html", "<!DOCTYPE html><html><body><h2>ESP32 WebSocket</h2><script>const socket = new WebSocket('ws://' + location.hostname + '/ws'); socket.onopen = () => { console.log('Connected'); socket.send('Hi from browser'); };</script></body></html>");
});

  Serial.begin(115200);
  initWiFiAP();
  initWebSocket();

  //Set initial Built-in LED state
  pinMode(Monitor_LED, OUTPUT);
  digitalWrite(Monitor_LED, LOW);
  Monitor_LED_State = false;



  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  if (millis() - tblink > 500) {
    tblink = millis();
    LED_Blink();
    Serial.println(WiFi.localIP());
  }
}

// put function definitions here:
void initWiFiAP(){
//Create WiFi AP
  WiFi.softAPConfig(staticIP, gateway, subnet); //Set static IP, gateway and subnet mask
  WiFi.softAP(ssid); //add password parameter if needed
  IPAddress IP = WiFi.softAPIP(); //Get the IP address of the AP
}

void initWebSocket(){
  ws.onEvent(onWebEvent); //Attach the WebSocket event handler
  server.addHandler(&ws); //Add the WebSocket handler to the server
}

void LED_Blink(){

    Monitor_LED_State = !Monitor_LED_State;
    digitalWrite(Monitor_LED, Monitor_LED_State ? HIGH : LOW);
}

void onWebEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  //Handle WebSocket events here
  if (type == WS_EVT_CONNECT) {
    Serial.println("WebSocket client connected");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("WebSocket client disconnected");
  } else if (type == WS_EVT_DATA) {
    Serial.printf("WebSocket data received: %.*s\n", len, data);
    // Process incoming data
  }
}