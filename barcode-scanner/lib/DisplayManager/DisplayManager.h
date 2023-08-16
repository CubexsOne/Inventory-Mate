#include <Arduino.h>

#ifndef DisplayManagerH
#define DisplayManagerH

class DisplayManager {
public:
  DisplayManager();
  void begin();
  void printWithInterface(String);
  void loop();
  void welcome();
};

#endif