#include "MqttClient.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include "secrets.h"

WiFiClient wifiClient;
PubSubClient pubSubClient(wifiClient);

// Variables
const char* mqtt_server = MQTT_HOST;
const int mqtt_port = 23218;
const char* mqtt_user = MQTT_USER;
const char* mqtt_password = MQTT_PASSWORD;
const char* mqtt_read_topic = MQTT_READ_TOPIC;
const char* mqtt_write_topic = MQTT_WRITE_TOPIC;
DisplayManager dm;

// Definitions
void callback(char*, byte*, unsigned int);

MqttClient::MqttClient() {}

void MqttClient::begin(DisplayManager displayManager) {
  dm = displayManager;

  pubSubClient.setServer(mqtt_server, mqtt_port);
  pubSubClient.setCallback(callback);
  pubSubClient.setKeepAlive(60);
}

void MqttClient::loop() {
  if (!pubSubClient.connected()) {
    reconnect();
  }
  pubSubClient.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  String product = "";
  for (int i = 0; i < length; i++) {
    product += (char)payload[i];
  }
  dm.printItem(product);
}

void MqttClient::reconnect() {
  Serial.println("Connecting to MQTT-Broker...");
  while (!pubSubClient.connected()) {
    if (pubSubClient.connect("Scanner_001_ClientID", mqtt_user, mqtt_password)) {
      Serial.println("Connected!");
      pubSubClient.subscribe(mqtt_read_topic);
    } else {
      Serial.println("Retrying in 5 seconds... rc: " + pubSubClient.state());
      delay(5000);
    }
  }
}

void MqttClient::sendMQTTMessage(String message) {
  if (!pubSubClient.connected()) {
    reconnect();
  }
  dm.printBarcode(message);
  pubSubClient.publish(mqtt_write_topic, message.c_str());
}