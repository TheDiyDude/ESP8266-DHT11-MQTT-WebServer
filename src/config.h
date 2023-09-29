
#pragma once

//#define LED D6
#define LED                          LED_BUILTIN
#define TEMPERATURE_POLLING_INTERVAL 1000*30
#define HUMIDITY_POLLING_INTERVAL    1000*30
#define DHTPIN                       D5
#define DHTTYPE                      DHT11   // DHT 11
                                             // DHT 22  (AM2302), AM2321
                                             // DHT 21  (AM2301)
 const char *mqtt_topics[] = {
    "/PoolSteuerung/PoolTemperatur",
    "/SmartMeter/EASTRON/SDM72DV2/ImportPower",
    "/SmartMeter/EASTRON/SDM72DV2/ExportPower",
    "/SmartMeter/EASTRON/SDM72DV2/L1_Power",
    "/SmartMeter/EASTRON/SDM72DV2/L2_Power",
    "/SmartMeter/EASTRON/SDM72DV2/L3_Power"};

char    mqtt_topic_temperature[100] = "/temperature";
char    mqtt_topic_humidity[100]    = "/humidity";
String  client_id                   = "";
char    payload[20]                 = "";

// --------------------------------------------------------------------------
