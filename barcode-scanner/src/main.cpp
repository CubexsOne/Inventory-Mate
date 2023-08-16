#include <Arduino.h>
#include "DisplayManager.h"
#include "MqttClient.h"
#include "WifiManager.h"

// Display
DisplayManager displayManager;

// MQTT
MqttClient mqttClient;

// Wifi
WifiManager wifiManager;

void setup() {
  Serial.begin(9600);
  displayManager.begin();
  mqttClient.begin();
}

void loop() {
  wifiManager.loop();
  displayManager.loop();
  mqttClient.loop();
}