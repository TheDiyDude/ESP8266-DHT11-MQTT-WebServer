#include "theDiyDude.h"

unsigned long    currentMillis     = 0L;
unsigned long    previousMillis    = 0L; 
int              ledState          = LOW;
bool             _mqtt             = true;

#define INTERVAL_MILLIS              1000L

extern configData data;


void blink_led(int pin, unsigned long time = INTERVAL_MILLIS)
{
    if (currentMillis - previousMillis >= time) {
        previousMillis = currentMillis;
        if (ledState == LOW) {
                ledState = HIGH;
        } else {
                ledState = LOW;
        }
        digitalWrite(pin, ledState);
    }
}

void updateMillisStart() {
    currentMillis = millis();
}

void readData() {                                 
  EEPROM.begin(512);
  debugln("Reading From EEPROM..");
  for (int i = 0; i < 50; i++) {                  
    if (char(EEPROM.read(i)) > 0) {
      data.ssid[i] += char(EEPROM.read(i));
    }
  }
  for (int i = 50; i < 100; i++) {                 
    if (char(EEPROM.read(i)) > 0) {
      data.password[i-50] += char(EEPROM.read(i));
    }
  }
  debug("Wifi ssid: "); 
  debugln(_ssid);
  debug("Wifi password: ");
  debugln(_pass);
  EEPROM.end();
}
void writeData(String a, String b) {               
  EEPROM.begin(512);                              
  debugln("Writing to EEPROM...");
  for (int i = 0; i < 50; i++) {                  
    EEPROM.write(i, a[i]);
  }
  for (int i = 50; i < 100; i++) {                 
    EEPROM.write(i, b[i - 50]);
    debugln(b[i]);
  }
  EEPROM.commit();
  EEPROM.end();
  debugln("Write Successfull");
}


void print_configuration(){
  Serial.printf("WIFI SSID  : %s\n",data.ssid);
  Serial.printf("WIFI PASS  : %s\n",data.password);
  Serial.printf("Hostname   : %s\n",data.hostname);
  Serial.printf("MQTT Broker: %s\n",data.mqtt_broker);
  Serial.printf("MQTT User  : %s\n",data.mqtt_username);
  Serial.printf("MQTT Pass  : %s\n",data.mqtt_password);
  Serial.printf("MQTT Port  : %s\n",data.mqtt_port);
  Serial.printf("MQTT Subs  : %s\n",data.mqtt_subscribe);
  Serial.printf("Size       : %d\n",sizeof(data));
}