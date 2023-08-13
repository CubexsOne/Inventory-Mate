#include <Arduino.h>

#include "WifiManager.h"

WifiManager wifiManager;

void setup() {
  Serial.begin(9600);
}

void loop() {
  wifiManager.loop();
}