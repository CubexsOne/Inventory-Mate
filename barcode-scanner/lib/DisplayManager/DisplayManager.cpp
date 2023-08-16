#include "DisplayManager.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// Variables
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
// auto-disable
bool isEnabled = true;
long timeOnEnable = 0;
int timeToDim = 3000;

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

void DisplayManager::printBarcode(String barcode) {
  isEnabled = true;
  timeOnEnable = millis();
  createBorder();
  printTitle();
  printText(40, 20, 1, "Barcode:");
  printText(10, 32, 1, barcode);
}

void DisplayManager::printItem(String item) {
  isEnabled = true;
  timeOnEnable = millis();
  createBorder();
  printTitle();
  printText(40, 20, 1, "Gescannt:");
  printText(10, 32, 1, item);
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
  printText(24, 8, 1, "Inventory Mate");
}

void printText(int16_t x, int16_t y, uint8_t textSize, String text) {
  oled.setCursor(x, y);
  oled.setTextSize(textSize);
  oled.setTextColor(WHITE);
  oled.print(text);
  oled.display();
}