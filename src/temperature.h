#pragma once
#include "theDiyDude.h"
#include <DHT.h>

#define TEMPERATURE_POLLING_INTERVAL 1000*30
#define HUMIDITY_POLLING_INTERVAL    1000*30

#define DHTPIN  D5
#define DHTTYPE DHT11   // DHT 11
                        // DHT 22  (AM2302), AM2321
                        // DHT 21  (AM2301)

void temperatureSetup();
float getTemp();
float getHumidity();
String temperatureTest();
