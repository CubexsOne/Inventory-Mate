#include "DisplayManager.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// Variables
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
// auto-disable
bool isEnabled = true;
long timeOnEnable = 0;
int timeToDim = 100 * 1000;

// Definitions
void initialize();
void createBorder();
void disableDisplay();
void printTitle();
void printText(int16_t, int16_t, uint8_t, String);

DisplayManager::DisplayManager() {}

void DisplayManager::begin() {
  oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  oled.setTextWrap(true);
  initialize();
}

void DisplayManager::loop() {
  if (isEnabled && (timeOnEnable + timeToDim < millis())) {
    disableDisplay();
  }
}

void DisplayManager::welcome() {
  isEnabled = true;
  timeOnEnable = millis();
  oled.clearDisplay();
  printText(24, 0, 2, "Welcome");
  printText(52, 16, 2, "to");
  printText(12, 32, 2, "Inventory");
  printText(40, 48, 2, "Mate");
}

void DisplayManager::printWithInterface(String text) {
  isEnabled = true;
  timeOnEnable = millis();
  createBorder();
  printTitle();
  printText(8, 28, 1, text);
}

void initialize() {
  isEnabled = true;
  timeOnEnable = millis();
  createBorder();
  printText(6, 24, 2, "Booting...");
}

void createBorder() {
  oled.clearDisplay();
  for (int counter = 0; counter < 3; ++counter) {
    oled.drawRect(counter, counter, 128 - (counter * 2), 64 - (counter * 2), WHITE);
  }
  oled.display();
}

void disableDisplay() {
  isEnabled = false;
  oled.clearDisplay();
  oled.display();
}

void printTitle() {
  printText(8, 8, 2, "Inventory Mate");
}

void printText(int16_t x, int16_t y, uint8_t textSize, String text) {
  oled.setCursor(x, y);
  oled.setTextSize(textSize);
  oled.setTextColor(WHITE);
  oled.print(text);
  oled.display();
}