#include "DisplayManager.h"
#include <Adafruit_SSD1306.h>

// Variables
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
// auto-disable
bool isEnabled = true;
long timeOnEnable = 0;
int timeToDim = 3000*5;

// Definitions
void initialize();
void createBorder();
void disableDisplay();

DisplayManager::DisplayManager() {}

void DisplayManager::begin() {
  oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  initialize();
}

void DisplayManager::loop() {
  if (isEnabled && (timeOnEnable + timeToDim < millis())) {
    disableDisplay();
  }
}

void initialize() {
  isEnabled = true;
  timeOnEnable = millis();
  createBorder();
}

void createBorder() {
  oled.clearDisplay();
  for (int counter = 0; counter < 3; ++counter) {
    oled.drawRect(counter, counter, 128 - (counter * 2), 64 - (counter * 2), SSD1306_WHITE);
  }
  oled.display();
}

void disableDisplay() {
  isEnabled = false;
  oled.clearDisplay();
  oled.display();
}