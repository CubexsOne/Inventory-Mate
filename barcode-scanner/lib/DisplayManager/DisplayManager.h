#include <Arduino.h>

#ifndef DisplayManagerH
#define DisplayManagerH

class DisplayManager {
public:
  DisplayManager();
  void begin();
  void loop();
  void printBarcode(String);
  void printItem(String);
};

#endif