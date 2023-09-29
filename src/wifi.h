
#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "theDiyDude.h"
#include "util.h"

void wifi_setup();
void ap_mode();
bool testWifi();
void scanNetworks();
