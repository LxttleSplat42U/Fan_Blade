#include <WifiSetup.h>


// WiFi credentials and static IP configuration

//const char* password = ""; //Password if required

IPAddress local_IP(192, 168, 4, ESP_ID); // Static IP address for this ESP32
IPAddress gateway(192, 168, 4, 1); // Gateway IP address
IPAddress subnet(255, 255, 255, 0); // Subnet mask

void connectToWifi() {
  // Attempt to connect to WiFi network
  Serial.print("Connecting to WiFi network: ");
  Serial.println(String(ssid) + " with password: " + String(password));

  // Configure static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);

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