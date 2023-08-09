#include <Arduino.h>

#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

class MQTTClient {
public:
    MQTTClient();
    void begin();
    void loop();
    void sendMQTTMessage(String);
private:
    void reconnect();
};

#endif