#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "secrets.h"
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <Wire.h>

// Declare Wifi
const char* wifi_ssid = WIFI_SSID;
const char* wifi_password = WIFI_PASSWORD;
WiFiClient wifiClient;
void setup_wifi();

// Declare MQTT
const char* mqtt_server = MQTT_HOST;
const int mqtt_port = 23218;
const char* mqtt_user = MQTT_USER;
const char* mqtt_password = MQTT_PASSWORD;
const char* mqtt_read_topic = MQTT_READ_TOPIC;
const char* mqtt_write_topic = MQTT_WRITE_TOPIC;
PubSubClient mqttClient(wifiClient);
void setup_mqtt();
void reconnect();
void callback(char*, byte*, unsigned int);
void sendMQTTMessage(String);

// Declare Pins
const int BUTTON_PIN = 25;
const int RED_LED_PIN = 26;

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
  setup_wifi();

  // SETUP MQTT
  setup_mqtt();

  // SETUP Pins
  pinMode(BUTTON_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, HIGH);

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
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  // Read Button
  handleButton();


  // Read BarcodeScanner
  if (barcodScanner.available()) {
    while (barcodScanner.available()) {
      char input = barcodScanner.read();
      if (input == '\n') {
        String message = createMessage(barcode);
        sendMQTTMessage(message);
        barcode = "";
        return;
      }
      if (input != '\r') {
        barcode += input;
      }
    }
  }
}

// WIFI
void setup_wifi() {
  delay(10);
  WiFi.begin(wifi_ssid, wifi_password);

  Serial.print("Connecting to Wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");
}

// MQTT
void setup_mqtt() {
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
    String product = "";
    for (int i = 0; i < length; i++) {
      product += (char)payload[i];
    }
    drawInterface();
    drawProductName(product);
}

void reconnect() {
  Serial.println("Connecting to MQTT-Broker...");
  while (!mqttClient.connected()) {
    if (mqttClient.connect("Scanner_001_ClientID", mqtt_user, mqtt_password)) {
      Serial.println("Connected!");
      mqttClient.subscribe(mqtt_read_topic);
      digitalWrite(RED_LED_PIN, LOW);
    } else {
      Serial.println("Retrying in 5 seconds... rc: " + mqttClient.state());
      digitalWrite(RED_LED_PIN, HIGH);
      delay(5000);
    }
  }
}

void sendMQTTMessage(String message) {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.publish(mqtt_write_topic, message.c_str());
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