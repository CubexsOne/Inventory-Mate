#include <WiFi.h>
#include "secrets.h"
#include "wifi_manager.h"

// Variables
const char* wifi_ssid = WIFI_SSID;
const char* wifi_password = WIFI_PASSWORD;

WifiManager::WifiManager() {}

void WifiManager::connect() {
  WiFi.begin(wifi_ssid, wifi_password);

  Serial.print("Connecting to Wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");
}