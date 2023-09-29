#include <Arduino.h>
#include "theDiyDude.h"
#include "temperature.h"

DHT dht(DHTPIN, DHTTYPE);

String          output;
float           previousTemperature         =   0.0;
float           previousHumidity            =   0.0;
unsigned long   previousMillisGetTemp       =   99999L; 
unsigned long   previousMillisGetHumidity   =   99999L; 

extern unsigned long currentMillis;

void halt() {
    delay(100000);
}
void temperatureSetup() {
    Serial.println(F("DHTxx starting"));
    dht.begin();
    Serial.println(temperatureTest());
}

float getTemp(){
    float value;
    if (currentMillis - previousMillisGetTemp >= TEMPERATURE_POLLING_INTERVAL) {
        previousMillisGetTemp = currentMillis;
        value = dht.readTemperature();
        if (isnan(value)) {
            debugln(F("Failed to read from DHT sensor!"));
            return -100.99;
        }
        previousTemperature = value;
    } else { value = previousTemperature; }

    return value;
}

float getHumidity(){
    float value;
    if (currentMillis - previousMillisGetHumidity >= HUMIDITY_POLLING_INTERVAL) {
        previousMillisGetHumidity = currentMillis;
        value = dht.readHumidity();
        if (isnan(value)) {
            debugln(F("Failed to read from DHT sensor!"));
            return -100.99;
        }
        previousHumidity = value;
    } else { value = previousHumidity; }
    return value;
}

String temperatureTest() {

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);

    if (isnan(h) || isnan(t) || isnan(f)) {
        debugln(F("Failed to read from DHT sensor!"));
        //return;
    }

    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);

    output = F("Humidity: ");
    output += h;
    output += F("%  Temperature: ");
    output += t;
    output += F("°C ");
    output += f;
    output += F("°F  Heat index: ");
    output += hic;
    output += F("°C ");
    output += hif;
    output += F("°F");

    return output;
}