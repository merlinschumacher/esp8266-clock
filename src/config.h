#ifndef config_h
#define config_h

#include <Arduino.h>
#include <map>
#include <SPI.h>
#include <FS.h>
#include <ArduinoJson.h>

struct ConfigData
{
    char timeserver[64];
    int timezone;
    int32 hourColor;
    int32 minuteColor;
    int32 secondColor;
    int brightnessDay;
    int brightnessDimmed;
};

class Config
{
public:
    Config();
    ConfigData config;
    void save();
    void load();
    StaticJsonDocument<512> configToJSON();
    void JSONToConfig(StaticJsonDocument<512>);

private:
    void _loadFile();
    FS *_filesystem = &SPIFFS;
};

#endif