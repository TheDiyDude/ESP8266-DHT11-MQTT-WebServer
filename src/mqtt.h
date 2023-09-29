
#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include "temperature.h"

void callback(char *topic, byte *payload, unsigned int length);
class MQTT {
    bool connected = false;

public:
    MQTT() {};
    void begin();
    void checkConnection();
    void publish(const char *topic, char *buffer);
    void loop();

private:
};