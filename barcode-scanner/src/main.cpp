#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include "secrets.h"
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <Wire.h>

// Declare functions
void setup_wifi();
void setup_mqtt();
void reconnect();
void callback(char*, byte*, unsigned int);
void sendMQTTMessage(const char*);

// Declare Wifi
const char* wifi_ssid = WIFI_SSID;
const char* wifi_password = WIFI_PASSWORD;
WiFiClient espClient;

// Declare MQTT
const char* mqtt_server = MQTT_HOST;
const int mqtt_port = 1883;
const char* mqtt_user = MQTT_USER;
const char* mqtt_password = MQTT_PASSWORD;
const char* mqtt_read_topic = MQTT_READ_TOPIC;
const char* mqtt_write_topic = MQTT_WRITE_TOPIC;
PubSubClient client(espClient);

// Declare Pins
const int BUTTON_PIN = 25;
const int RED_LED_PIN = 26;

// Declare Barcode Scanner
SoftwareSerial barcodScanner(16, 17);
String barcode = "";

// Declare OLED Display
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  // SETUP Serial
  Serial.begin(9600);
  barcodScanner.begin(9600);

  // SETUP Wifi
  setup_wifi();

  // SETUP Pins
  pinMode(BUTTON_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  // SETUP Display
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();
}

void loop() {
  // Etablish MQTT Connection
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read Button
  // int buttonState = digitalRead(BUTTON_PIN);
  // if (buttonState == HIGH) {
  //   Serial.println("Button pressed");
  // }

  // Enable / Disable LED
  // digitalWrite(RED_LED_PIN, HIGH);
  // digitalWrite(RED_LED_PIN, LOW);

  // Read BarcodeScanner
  // if (barcodScanner.available()) {
  //   while (barcodScanner.available()) {
  //     char input = barcodScanner.read();
  //     barcode =+ input;
  //     Serial.print(input);
  //     if (input == '\n') {
  //       // Send Item HERE
  //       Serial.print("ENTER!");
  //       barcode = "";
  //     }
  //   }
  // }
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
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (topic == mqtt_read_topic) {
    Serial.print("Message received: ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  }
}

void reconnect() {
  Serial.println("Connecting to MQTT-Broker...");
  while (!client.connected()) {
    if (client.connect("Scanner_001_ClientID", mqtt_user, mqtt_password)) {
      Serial.println("Connected!");
      client.subscribe(mqtt_read_topic);
    } else {
      Serial.println("Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void sendMQTTMessage(const char* message) {
  if (!client.connected()) {
    reconnect();
  }
  client.publish(mqtt_write_topic, message);
}