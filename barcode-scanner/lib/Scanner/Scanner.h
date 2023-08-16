#include <Arduino.h>
#include "MqttClient.h"

#ifndef ScannerH
#define ScannerH

class Scanner {
public:
  Scanner();
  void begin(MqttClient);
  void loop();
};

#endif