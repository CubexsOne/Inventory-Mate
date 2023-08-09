#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include <SoftwareSerial.h>
#include <Wire.h>

#include "wifi_manager.h"
#include "mqtt_client.h"


// Wifi
WifiManager wifiManager;

// MQTT
MQTTClient mqttClient;

// Declare Pins
const int BUTTON_PIN = 15;

// Declare Barcode Scanner
SoftwareSerial barcodScanner(16, 17);

// Declare OLED Display
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);
void drawInterface();
void drawProductName(String);

// Scanner
enum ScanMode {
  USE,
  NEW
};
ScanMode currentMode = USE;
String barcode = "";
String createMessage(String);
const char *modeToString(ScanMode);

// Button
int buttonState = HIGH;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
void handleButton();

void setup() {
  // SETUP Serial
  Serial.begin(9600);
  barcodScanner.begin(9600);

  // SETUP Wifi
  wifiManager.connect();

  // SETUP MQTT
  mqttClient.begin();

  // SETUP Pins
  pinMode(BUTTON_PIN, INPUT);

  // SETUP Display
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();
  drawInterface();
  delay(3000);
  display.clearDisplay();
  display.display();
}

void loop() {
  // Etablish MQTT Connection
  mqttClient.loop();

  // Read Button
  handleButton();


  // Read BarcodeScanner
  if (barcodScanner.available()) {
    while (barcodScanner.available()) {
      char input = barcodScanner.read();
      if (input == '\n') {
        String message = createMessage(barcode);
        mqttClient.sendMQTTMessage(message);
        barcode = "";
        return;
      }
      if (input != '\r') {
        barcode += input;
      }
    }
  }
}

// Message
String createMessage(String barcode) {
  StaticJsonDocument<200> json;
  String jsonString = "";

  json["mode"] = modeToString(currentMode);
  json["code"] = barcode;

  serializeJson(json, jsonString);

  return jsonString;
}

const char* modeToString(ScanMode mode) {
  if (mode == NEW) {
    return "NEW";
  }
  return "USE";
}

void nextMode() {
  if (currentMode == USE) {
    currentMode = NEW;
    return;
  }

  currentMode = USE;
}

// Display
void drawInterface() {
  display.clearDisplay();
  for (int counter = 0; counter < 3; ++counter) {
    display.drawRect(counter, counter, 128 - (counter * 2), 64 - (counter * 2), SSD1306_WHITE);
  }

  display.setCursor(8, 8);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  display.print("Mode:");
  display.print(modeToString(currentMode));
  display.display();
}

void drawProductName(String product) {
  display.setCursor(8, 28);
  display.setTextSize(1);
  display.print(product);
  display.display();

  delay(2500);
  display.clearDisplay();
  display.display();
}

// Button
void handleButton() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        nextMode();
        drawInterface();
        drawProductName("");
      }
    }
  }
  lastButtonState = reading;
}