#include "Scanner.h"

#include <SoftwareSerial.h>
#include <Wire.h> 
#include "MqttClient.h"

// Variables
String barcode = "";
SoftwareSerial barcodeScanner(16, 17);
MqttClient sMqtt;

Scanner::Scanner() {}

void Scanner::begin(MqttClient mqttClient) {
  sMqtt = mqttClient;
  barcodeScanner.begin(9600);
}

void Scanner::loop() {
  if (barcodeScanner.available()) {
    while (barcodeScanner.available()) {
      char input = barcodeScanner.read();
      if (input == '\n') {
        sMqtt.sendMQTTMessage(barcode);
        barcode = "";
        return;
      }
      if (input != '\r') {
        barcode += input;
      }
    }
  }
}