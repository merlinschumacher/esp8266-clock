#include "config.hpp"

#if defined(ESP8266)
#define MAXPINS 16
#elif definded(ESP32)
#define MAXPINS 35
#endif
#define MAXLEDS 360
Config::Config()
{
}

uint32_t _clampInt(uint32_t val, uint32_t min, uint32_t max)
{
        const uint32_t t = val < min ? min : val;
        return t > max ? max : t;
}

void Config::configToJSON(JsonDocument &doc, bool skipSensitiveData)
{
        Config::locked = true;
        if (!skipSensitiveData)
        {
                doc["hostname"] = config.hostname;
                doc["timeserver"] = config.timeserver;
                doc["timezone"] = config.timezone;
        }

        doc["hourColor"] = config.hourColor;
        doc["minuteColor"] = config.minuteColor;
        doc["secondColor"] = config.secondColor;

        doc["hourColorDimmed"] = config.hourColorDimmed;
        doc["minuteColorDimmed"] = config.minuteColorDimmed;
        doc["secondColorDimmed"] = config.secondColorDimmed;

        doc["hourDot"] = config.hourDot;
        doc["hourSegment"] = config.hourSegment;
        doc["hourQuarter"] = config.hourQuarter;

        doc["hourDotColor"] = config.hourDotColor;
        doc["hourSegmentColor"] = config.hourSegmentColor;
        doc["hourQuarterColor"] = config.hourQuarterColor;

        doc["hourDotColorDimmed"] = config.hourDotColorDimmed;
        doc["hourSegmentColorDimmed"] = config.hourSegmentColorDimmed;
        doc["hourQuarterColorDimmed"] = config.hourQuarterColorDimmed;

        doc["dayMonth"] = config.dayMonth;
        doc["dayColor"] = config.dayColor;
        doc["monthColor"] = config.monthColor;
        doc["weekdayColor"] = config.weekdayColor;
        doc["dayColorDimmed"] = config.dayColorDimmed;
        doc["monthColorDimmed"] = config.monthColorDimmed;
        doc["weekdayColorDimmed"] = config.weekdayColorDimmed;
        doc["monthOffset"] = config.monthOffset + 1;
        doc["dayOffset"] = config.dayOffset + 1;
        doc["weekdayOffset"] = config.weekdayOffset + 1;

        doc["nightTimeBegins"] = config.nightTimeBegins;
        doc["nightTimeEnds"] = config.nightTimeEnds;

        doc["hourHandStyle"] = config.hourHandStyle;
        doc["hourLight"] = config.hourLight;
        doc["blendColors"] = config.blendColors;
        doc["fluidMotion"] = config.fluidMotion;

        doc["alarmTime"] = config.alarmTime;
        doc["alarmActive"] = config.alarmActive;

        doc["ledPin"] = config.ledPin;
        doc["ledCount"] = config.ledCount;
        doc["ledRoot"] = config.ledRoot + 1;

        doc["bgLight"] = config.bgLight;
        doc["bgLedPin"] = config.bgLedPin;
        doc["bgLedCount"] = config.bgLedCount;
        doc["bgColor"] = config.bgColor;
        doc["bgColorDimmed"] = config.bgColorDimmed;

        doc["language"] = config.language;

        if (!skipSensitiveData)
        {
                doc["mqttActive"] = config.mqttActive;
                doc["mqttServer"] = config.mqttServer;
                doc["mqttUser"] = config.mqttUser;
                doc["mqttPassword"] = config.mqttPassword;
                doc["mqttPort"] = config.mqttPort;
                doc["mqttBaseTopic"] = config.mqttBaseTopic;
        }

#if defined(UART_MODE) || defined(DMA_MODE)
        doc["pinsLocked"] = true;
#else
        doc["pinsLocked"] = false;
#endif
        Config::locked = false;
}

bool Config::JSONToConfig(JsonDocument &doc, bool skipSensitiveData)
{

        Config::locked = true;
        if (doc.containsKey("hostname"))
        {
                strlcpy(config.hostname,
                        doc["hostname"],
                        sizeof(config.hostname));
        }
        else
        {
#if defined(ESP8266)
                uint32_t chipid = ESP.getChipId();
#elif defined(ESP32)
                uint64_t chipid = ESP.getEfuseMac();
#endif
                snprintf(config.hostname, sizeof(config.hostname), "ESPCLOCK-%06X", chipid);
        }

        strlcpy(config.timeserver,
                doc["timeserver"] | "pool.ntp.org",
                sizeof(config.timeserver));
        strlcpy(config.timezone,
                doc["timezone"] | "Europe/Berlin",
                sizeof(config.timezone));

        strlcpy(config.hourColor,
                doc["hourColor"] | "#FF0000",
                sizeof(config.hourColor));
        strlcpy(config.minuteColor,
                doc["minuteColor"] | "#00FF00",
                sizeof(config.minuteColor));
        strlcpy(config.secondColor,
                doc["secondColor"] | "#0000FF",
                sizeof(config.secondColor));

        strlcpy(config.hourColorDimmed,
                doc["hourColorDimmed"] | "#770000",
                sizeof(config.hourColorDimmed));
        strlcpy(config.minuteColorDimmed,
                doc["minuteColorDimmed"] | "#007700",
                sizeof(config.minuteColorDimmed));
        strlcpy(config.secondColorDimmed,
                doc["secondColorDimmed"] | "#000077",
                sizeof(config.secondColorDimmed));

        strlcpy(config.hourHandStyle,
                doc["hourHandStyle"] | "simple",
                sizeof(config.hourHandStyle));
        config.hourDot = doc["hourDot"] | false;
        config.hourSegment = doc["hourSegment"] | false;
        config.hourQuarter = doc["hourQuarter"] | false;

        strlcpy(config.hourDotColor,
                doc["hourDotColor"] | "#010101",
                sizeof(config.hourDotColor));
        strlcpy(config.hourSegmentColor,
                doc["hourSegmentColor"] | "#010000",
                sizeof(config.hourSegmentColor));
        strlcpy(config.hourQuarterColor,
                doc["hourQuarterColor"] | "#000001",
                sizeof(config.hourQuarterColor));

        strlcpy(config.hourDotColorDimmed,
                doc["hourDotColorDimmed"] | "#000000",
                sizeof(config.hourDotColorDimmed));
        strlcpy(config.hourSegmentColorDimmed,
                doc["hourSegmentColorDimmed"] | "#000000",
                sizeof(config.hourSegmentColorDimmed));
        strlcpy(config.hourQuarterColorDimmed,
                doc["hourQuarterColorDimmed"] | "#000000",
                sizeof(config.hourQuarterColorDimmed));

        config.dayMonth = doc["dayMonth"] | false;
        strlcpy(config.dayColor,
                doc["dayColor"] | "#FF00CE",
                sizeof(config.dayColor));
        strlcpy(config.monthColor,
                doc["monthColor"] | "#FFFA00",
                sizeof(config.monthColor));
        strlcpy(config.weekdayColor,
                doc["weekdayColor"] | "#00FFC7",
                sizeof(config.weekdayColor));

        strlcpy(config.dayColorDimmed,
                doc["dayColorDimmed"] | "#7B0063",
                sizeof(config.dayColorDimmed));
        strlcpy(config.monthColorDimmed,
                doc["monthColorDimmed"] | "#575500",
                sizeof(config.monthColorDimmed));
        strlcpy(config.weekdayColorDimmed,
                doc["weekdayColorDimmed"] | "#00755B",
                sizeof(config.weekdayColorDimmed));

        config.nightTimeBegins = doc["nightTimeBegins"] | 1320;
        config.nightTimeBegins = _clampInt(config.nightTimeBegins, 0, 1440);

        config.nightTimeEnds = doc["nightTimeEnds"] | 480;
        config.nightTimeEnds = _clampInt(config.nightTimeEnds, 0, 1440);

        config.hourLight = doc["hourLight"] | false;
        config.blendColors = doc["blendColors"] | true;
        config.fluidMotion = doc["fluidMotion"] | true;

        config.alarmActive = doc["alarmActive"] | false;
        config.alarmTime = doc["alarmTime"] | 480;

        config.bgLight = doc["bgLight"] | false;
        strlcpy(config.bgColor,
                doc["bgColor"] | "#000000",
                sizeof(config.bgColor));
        strlcpy(config.bgColorDimmed,
                doc["bgColorDimmed"] | "#000000",
                sizeof(config.bgColorDimmed));

#if defined(BITBANG_MODE) || defined(DEBUG_BUILD) || defined(ESP32)
        config.bgLedPin = doc["bgLedPin"] | 15;
        config.bgLedPin = _clampInt(config.bgLedPin, 0, MAXPINS);
#elif defined(UART_MODE)
        if (!skipSensitiveData)
                config.bgLedPin = 1;
#elif defined(DMA_MODE)
        if (!skipSensitiveData)
                config.bgLedPin = 2;
#endif

        config.bgLedCount = doc["bgLedCount"] | 60;
        config.bgLedCount = _clampInt(config.bgLedCount, 0, MAXLEDS);

#if defined(BITBANG_MODE) || defined(DEBUG_BUILD) || defined(ESP32)
        config.ledPin = doc["ledPin"] | 4;
        config.ledPin = _clampInt(config.ledPin, 0, MAXPINS);
#elif defined(UART_MODE)
        if (!skipSensitiveData)
                config.ledPin = 2;
#elif defined(DMA_MODE)
        if (!skipSensitiveData)
                config.ledPin = 3;
#endif

        config.ledCount = doc["ledCount"] | 60;
        config.ledCount = _clampInt(config.ledCount, 0, MAXLEDS);

        config.ledRoot = doc["ledRoot"] | 1;
        config.ledRoot = _clampInt(config.ledRoot, 1, MAXLEDS);

        config.dayOffset = doc["dayOffset"] | 1;
        config.dayOffset = _clampInt(config.dayOffset, 1, MAXLEDS);

        config.monthOffset = doc["monthOffset"] | 1;
        config.monthOffset = _clampInt(config.monthOffset, 1, MAXLEDS);

        config.weekdayOffset = doc["weekdayOffset"] | 1;
        config.weekdayOffset = _clampInt(config.weekdayOffset, 1, MAXLEDS);

        strlcpy(config.language,
                doc["language"] | "en",
                sizeof(config.language));

        if (!skipSensitiveData)
        {
                config.mqttActive = doc["mqttActive"] | false;
                strlcpy(config.mqttServer,
                        doc["mqttServer"] | "mqtthost",
                        sizeof(config.mqttServer));
                strlcpy(config.mqttUser,
                        doc["mqttUser"] | "mqttuser",
                        sizeof(config.mqttUser));
                strlcpy(config.mqttUser,
                        doc["mqttUser"] | "username",
                        sizeof(config.mqttUser));
                strlcpy(config.mqttPassword,
                        doc["mqttPassword"] | "password",
                        sizeof(config.mqttPassword));
                config.mqttPort = doc["mqttPort"] | 1883;
                config.mqttPort = _clampInt(config.mqttPort, 1, 65535);
        }
        char defaultBaseTopic[144] = "espneopixelclock/";
        snprintf(defaultBaseTopic, sizeof(defaultBaseTopic), "espneopixelclock/%s", config.hostname);

        strlcpy(config.mqttBaseTopic,
                doc["mqttBaseTopic"] | defaultBaseTopic,
                sizeof(config.mqttBaseTopic));

        config.ledRoot--;
        config.dayOffset--;
        config.monthOffset--;
        config.weekdayOffset--;

        if (doc["reset"] == true)
        {
                Config::_resetRequest = true;
        }
        else
        {
                Config::_resetRequest = false;
        };

        Config::locked = false;

        if (doc["saveData"] == true)
        {
                return true;
        }
        else
        {
                return false;
        };
}

void Config::load()
{
        // Open file for reading
        Config::locked = true;

#if defined(ESP8266)
        File sourcefile = LittleFS.open("data.json", "r");
#elif defined(ESP32)
        File sourcefile = SPIFFS.open("/data.json", "r");
#endif

        // Allocate a temporary JsonDocument
        // Don't forget to change the capacity to match your requirements.
        // Use arduinojson.org/v6/assistant to compute the capacity.
        DynamicJsonDocument doc(2048);

        // Deserialize the JSON document
        DeserializationError error = deserializeJson(doc, sourcefile);
        doc.shrinkToFit();
        Config::JSONToConfig(doc);
        if (error)
        {
                sourcefile.close();
                Serial.println(F("Failed to read file, using default configuration"));
                Config::save();
                return;
        }
        // Copy values from the JsonDocument to the Config
        // Close the file (Curiously, File's destructor doesn't close the file)
        sourcefile.close();
        Config::locked = false;
}

void Config::save()
{
        bool format = false;
        // Delete existing file, otherwise the configuration is appended to the file
        Config::locked = true;
#if defined(ESP8266)
        File targetfile = LittleFS.open("data.json", "w");
#elif defined(ESP32)
        File targetfile = SPIFFS.open("/data.json", "w");
#endif

        // Open file for writing
        if (!targetfile)
        {
                Serial.println(F("Failed to create config file. Reformatting FS and rebooting."));
                format = true;

        }

        DynamicJsonDocument doc(2048);
        Config::configToJSON(doc);
        doc.shrinkToFit();
        // Serialize JSON to file
        if (serializeJson(doc, targetfile) == 0)
        {
                Serial.println(F("Failed to write to config file. Reformatting FS and rebooting."));
                format = true;

        }
        if (format) {
#if defined(ESP8266)
                LittleFS.format();
                ESP.restart();
#elif defined(ESP32)
                SPIFFS.format();
                ESP.restart();
#endif
        }

        // Close the file
        targetfile.close();
        Config::locked = false;
        Config::forceReset = Config::_resetRequest;
}
