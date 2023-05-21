// Sender board code with OLED display
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "esp_now.h"

#define OLED_RESET -1
const uint8_t RECEIVER_MAC_ADDRESS[] = {0x30, 0xAE, 0xA4, 0x43, 0x03, 0x38};
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    return;
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Sender board");
  display.display();

  // Initialize ESPNow
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESPNow");
    return;
  }

  // Set up ESPNow peer
  esp_now_peer_info_t peer;
  memset(&peer, 0, sizeof(esp_now_peer_info_t));
  memcpy(peer.peer_addr, RECEIVER_MAC_ADDRESS, 6);
  peer.channel = 0;
  peer.ifidx = WiFi.getMode() == WIFI_MODE_AP ? WIFI_IF_AP : WIFI_IF_STA;
  peer.encrypt = false;
  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("Error adding ESPNow peer");
    return;
  }
}

void loop() {
  // Create packet to send
  uint8_t data[] = {0x01, 0x02, 0x03};
  
  // Send packet and wait for acknowledgement
  uint32_t start_time = millis();
  esp_err_t result = esp_now_send(RECEIVER_MAC_ADDRESS, data, sizeof(data));
  if (result == ESP_OK) {
    uint8_t ack_data[] = {0x04, 0x05, 0x06};
    esp_now_send(RECEIVER_MAC_ADDRESS, ack_data, sizeof(ack_data));
    uint32_t end_time = millis();
    uint32_t elapsed_time = end_time - start_time;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Round-trip time:");
    display.setCursor(0, 16);
    display.print(elapsed_time);
    display.print(" ms");
    display.setCursor(0, 32);
    display.print("Signal strength:");
    display.setCursor(0, 48);
    display.print(WiFi.RSSI());
    display.print(" dBm");
    display.display();
  } else {
    Serial.println("Error sending ESPNow packet");
  }
  
  delay(5000);
}
