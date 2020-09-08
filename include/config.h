#ifndef config_h
#define config_h
#include <ArduinoJson.h>
#if defined(ESP8266)
#include <FS.h>
#include <LittleFS.h>
#elif defined(ESP32)
#include <SPIFFS.h>
#endif

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

    char hourDotColor[8];
    char hourSegmentColor[8];
    char hourQuarterColor[8];

    char hourDotColorDimmed[8];
    char hourSegmentColorDimmed[8];
    char hourQuarterColorDimmed[8];

    char nightTimeBegins[6];
    char nightTimeEnds[6];

    bool hourLight;
    bool blendColors;
    bool fluidMotion;

    bool alarmActive;
    char alarmTime[6];

    bool bgLight;
    char bgColor[8];
    char bgColorDimmed[8];
    uint32_t bgLedPin;
    uint32_t bgLedCount;

    uint32_t ledPin;
    uint32_t ledCount;
    uint32_t ledRoot;
};

class Config
{
public:
    Config();
    ConfigData config;
    void save();
    void load();
    StaticJsonDocument<2048> configToJSON();
    bool JSONToConfig(StaticJsonDocument<2048> doc);

private:
    void _loadFile();
    String _getHostname();
};

#endif //config_h