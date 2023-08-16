#include <Arduino.h>
#include "DisplayManager.h"
#include "MqttClient.h"
#include "Scanner.h"
#include "WifiManager.h"

// Display
DisplayManager displayManager;

// MQTT
MqttClient mqttClient;

// Scanner
Scanner scanner;

// Wifi
WifiManager wifiManager;

void setup() {
  Serial.begin(9600);
  displayManager.begin();
  mqttClient.begin(displayManager);
  scanner.begin(mqttClient);
}

void loop() {
  wifiManager.loop();
  displayManager.loop();
  mqttClient.loop();
  scanner.loop();
}