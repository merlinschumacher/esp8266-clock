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

    bool hourDot;
    bool hourSegment;
    bool hourQuarter;

    char hourDotColor[8];
    char hourSegmentColor[8];
    char hourQuarterColor[8];
    char hourDotColorDimmed[8];
    char hourSegmentColorDimmed[8];
    char hourQuarterColorDimmed[8];

    bool dayMonth;
    uint32_t monthOffset;
    uint32_t dayOffset;
    uint32_t weekdayOffset;

    char monthColor[8];
    char dayColor[8];
    char weekdayColor[8];

    char monthColorDimmed[8];
    char dayColorDimmed[8];
    char weekdayColorDimmed[8];

    uint16_t nightTimeBegins;
    uint16_t nightTimeEnds;

    char hourHandStyle[8];
    bool hourLight;
    bool blendColors;
    bool fluidMotion;

    bool alarmActive;
    uint32_t alarmTime;

    bool bgLight;
    char bgColor[8];
    char bgColorDimmed[8];
    uint32_t bgLedPin;
    uint32_t bgLedCount;

    uint32_t ledPin;
    uint32_t ledCount;
    uint32_t ledRoot;

    char language[3];

    bool mqttActive;
    bool mqttTLS;
    char mqttServer[64];
    char mqttUser[128];
    char mqttPassword[128];
    char mqttBaseTopic[128];
    char mqttFingerprint[128];
    uint16_t mqttPort;

    bool tlsBundleLoaded;
};

class Config
{
public:
    Config();
    ConfigData config = {};
    void save();
    void load();
    void configToJSON(JsonDocument &doc, bool skipSensitiveData = false);
    bool JSONToConfig(JsonDocument &doc, bool skipSensitiveData = false);
    bool locked = false;
    bool forceReset = false;
    bool tainted = false;

private:
    bool _resetRequest = false;
    uint8_t _validateInt(uint8_t val, uint8_t min, uint8_t max);
};

#endif //config_h