#include <Arduino.h>
#include "WifiManager.h"
#include "MqttClient.h"

// Wifi
WifiManager wifiManager;

// MQTT
MqttClient mqttClient;

void setup() {
  Serial.begin(9600);
}

void loop() {
  wifiManager.loop();
  mqttClient.loop();
}