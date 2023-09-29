#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include "theDiyDude.h"

class util
{
    int eep_adr = 0;

public:
    util(){};
    void clearEEPROM();
    void readConfigurationFromEEPROM();
    void writeConfigurationToEEPROM();
    void setEepAdr(int value);
    int getEepAdr();
};