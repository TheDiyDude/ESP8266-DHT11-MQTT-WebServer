#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include <EEPROM.h>

#ifdef DEBUG
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

struct configData
{
    char ssid[50];
    char password[50];
    char hostname[50];
    char mqtt_broker[50];
    char mqtt_username[50];
    char mqtt_password[50];
    char mqtt_port[5];
    char mqtt_subscribe[500];
};

#include "util.h"

void blink_led(int pin, unsigned long time);
void updateMillisStart();
void readData();
void writeData(String a, String b);
void print_configuration();
