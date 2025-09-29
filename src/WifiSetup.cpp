#include <WifiSetup.h>
#include <Websocket.h>

// WiFi credentials and static IP configuration
const char* ssid = "Holo3D"; //Main ESP32 Access Point SSID
//const char* password = ""; //Password if required

IPAddress local_IP(192, 168, 4, 11);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

void connectToWifi() {
  // Attempt to connect to WiFi network
  Serial.print("Connecting to WiFi network: ");
  Serial.println(ssid);

  // Configure static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid); //, password); // Uncomment password if needed

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Searching for WiFi...");
    delay(500);
    digitalWrite(BUILTIN_LED,!digitalRead(BUILTIN_LED)); //Blink LED until connected
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(BUILTIN_LED,HIGH); // Turn On LED when connected
  initWebSocketClient(); // Initialize WebSocket client
}