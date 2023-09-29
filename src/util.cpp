#include "util.h"

extern configData data;
extern String client_id;

void util::clearEEPROM()
{
    EEPROM.begin(512);
    debugln("Clearing EEPROM ");
    for (int i = 0; i < 512; i++)
    {
        debug(".");
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
    EEPROM.end();
}

void util::setEepAdr(int value){
  eep_adr = value;
}
int util::getEepAdr(){
  return eep_adr;
}
void util::writeConfigurationToEEPROM(){
  Serial.print("Writing to EEPROM\nWifi ssid: "); 
  Serial.println(data.ssid);
  Serial.print("Wifi password: ");
  Serial.println(data.password);
  EEPROM.begin(1000);
  EEPROM.put(0,data);
  EEPROM.commit();
  EEPROM.end();
  Serial.print("Writing to EEPROM -- FINISHED!"); 
}

void util::readConfigurationFromEEPROM(){
  if(getEepAdr()==0) {
    setEepAdr(sizeof(configData));
  }
  Serial.printf("Reading bytes: %d\n", getEepAdr());
  EEPROM.begin(1000);
  EEPROM.get(0,data);
  EEPROM.end();
  client_id = data.hostname;
  Serial.print("Reading from EEPROM\nWifi ssid: ");
  Serial.println(data.ssid);
  Serial.print("Wifi password: ");
  Serial.println(data.password);
}