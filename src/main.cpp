/*
  ESP8266 WebServer Module
   Own mDNS Name Broadcast
   DHT11 Sensor 
   MQTT subscriptions and send DHT11 values
   Most config is stored in EEPROM
   See config.h for subscribed topics and published topics
   Auto AP Mode in case of non available Wifi Networks or first setup
      Please connect to http://192.168.4.1 to configure
  
   Version history
    v0.1 2023-09-20 Initial commit
    v0.2 2023-09-22 Moved parts in classes
    v1.0 2023-09-29 Add EEPROM config load/save, cleanup unused stuff
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DEBUG

#include "config.h"
#include "util.h"
#include "theDiyDude.h"
#include "wifi.h"
#include "mqtt.h"
#include "website.h"
#include "webserver.h"
#include "temperature.h"

extern bool apmode;
extern ESP8266WebServer server;

MQTT       mqtt;
configData data;
util       util;


void setup()
{
  Serial.begin(115200);
  debugln("Booting");
  wifi_setup();
  if (!apmode) { 
    mqtt.begin();
  }
  pinMode(LED, OUTPUT);
  temperatureSetup();
}

void loop() {
  updateMillisStart();
  if (apmode) {
    server.handleClient(); 
    blink_led(LED,200);
  } else {
      if (WiFi.status() == WL_CONNECTED) { 
        MDNS.update();
        mqtt.loop();
        server.handleClient(); 
        mqtt.loop();
        mqtt.checkConnection();
        blink_led(LED,1000);
        mqtt.loop();
      }
  }
}