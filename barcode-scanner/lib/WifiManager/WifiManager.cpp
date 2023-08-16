#include "WifiManager.h"
#include <WiFi.h>
#include "secrets.h"

// Variables
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
bool isConnected = false;

WifiManager::WifiManager() {}

void WifiManager::loop() {
  if (WiFi.status() != WL_CONNECTED) {
    isConnected = false;
    WiFi.begin(ssid, password);
    Serial.println("Connecting to Wifi...");
  }

  while(WiFi.status() != WL_CONNECTED);

  if (WiFi.status() == WL_CONNECTED && !isConnected) {
    isConnected = true;
    Serial.println("Wifi connected!");
  }
}