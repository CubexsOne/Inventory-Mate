#include <Arduino.h>

#ifndef MqttClientH
#define MqttClientH

class MqttClient {
public:
    MqttClient();
    void begin();
    void loop();
    void sendMQTTMessage(String);
private:
    void reconnect();
};

#endif