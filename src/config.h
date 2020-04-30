#ifndef config_h
#define config_h

#include <Arduino.h>
#include <map>
#include <SPI.h>
#include <FS.h>
#include <ArduinoJson.h>

struct ConfigData
{
    char hostname[64];
    char timeserver[64];
    char timezone[64];
    char hourColor[8];
    char minuteColor[8];
    char secondColor[8];
    char hourColorDimmed[8];
    char minuteColorDimmed[8];
    char secondColorDimmed[8];
    int8 ledPin;
    int32 ledCount;
    int32 ledRoot;
};

class Config
{
public:
    Config();
    ConfigData config;
    void save();
    void load();
    StaticJsonDocument<512> configToJSON();
    void JSONToConfig(StaticJsonDocument<512> doc);

private:
    void _loadFile();
};

#endif