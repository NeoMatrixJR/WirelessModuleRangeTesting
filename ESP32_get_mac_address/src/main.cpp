#include <Arduino.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi to get the MAC address
  WiFi.mode(WIFI_STA);
  WiFi.begin("THEMATRIX_IoT", "6365matrix");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  // Print MAC address
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // Do nothing
}
