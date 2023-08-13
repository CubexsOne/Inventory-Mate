#include "WifiManager.h"
#include <WiFi.h>
#include "secrets.h"

// Variables
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
bool isConnected = false;

WifiManager::WifiManager() {}

void WifiManager::loop() {
  // TODO: Add display output
  if (WiFi.status() != WL_CONNECTED) {
    isConnected = false;
    WiFi.begin();
    Serial.println("Connecting to Wifi...");
  }

  if (WiFi.status() == WL_CONNECTED && !isConnected) {
    isConnected = true;
    Serial.println("Wifi connected!");
  }
}