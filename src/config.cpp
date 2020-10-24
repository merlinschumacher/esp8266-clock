#include "config.hpp"

Config::Config()
{
}

void Config::_getHostname(char *hostname)
{
        char hostn[64] = "ESPCLOCK-";
#if defined(ESP8266)
        uint32_t chipid = ESP.getChipId();
#elif defined(ESP32)
        uint64_t chipid = ESP.getEfuseMac();
#endif
        char chipidS[16];
        snprintf(chipidS, 64, "%08X", chipid);
        strlcpy(hostn, chipidS, sizeof(hostname));
        strlcpy(hostname, hostn, sizeof(hostname));
}

StaticJsonDocument<2048> Config::configToJSON()
{
        StaticJsonDocument<2048> doc;
        // Set the values in the document
        doc["hostname"] = config.hostname;
        doc["timeserver"] = config.timeserver;
        doc["timezone"] = config.timezone;

        doc["hourColor"] = config.hourColor;
        doc["minuteColor"] = config.minuteColor;
        doc["secondColor"] = config.secondColor;

        doc["hourColorDimmed"] = config.hourColorDimmed;
        doc["minuteColorDimmed"] = config.minuteColorDimmed;
        doc["secondColorDimmed"] = config.secondColorDimmed;

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

        return doc;
}

bool Config::JSONToConfig(StaticJsonDocument<2048> doc)
{

        if (doc.containsKey("hostname"))
        {
                strlcpy(config.hostname,
                        doc["hostname"],
                        sizeof(config.hostname));
        }
        else
        {
                char _host[64];
                _getHostname(_host);
                strlcpy(config.hostname,
                        _host,
                        sizeof(config.hostname));
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

        strlcpy(config.nightTimeBegins,
                doc["nightTimeBegins"] | "22:00",
                sizeof(config.nightTimeBegins));
        strlcpy(config.nightTimeEnds,
                doc["nightTimeEnds"] | "08:00",
                sizeof(config.nightTimeEnds));

        config.hourLight = doc["hourLight"] | false;
        config.blendColors = doc["blendColors"] | true;
        config.fluidMotion = doc["fluidMotion"] | true;

        config.alarmActive = doc["alarmActive"] | false;
        strlcpy(config.alarmTime,
                doc["alarmTime"] | "08:00",
                sizeof(config.alarmTime));

        config.bgLight = doc["bgLight"] | false;
        strlcpy(config.bgColor,
                doc["bgColor"] | "#000000",
                sizeof(config.bgColor));
        strlcpy(config.bgColorDimmed,
                doc["bgColorDimmed"] | "#000000",
                sizeof(config.bgColorDimmed));
        config.bgLedPin = doc["bgLedPin"] | 15;
        config.bgLedCount = doc["bgLedCount"] | 60;

        config.ledPin = doc["ledPin"] | 4;
        config.ledCount = doc["ledCount"] | 60;
        config.ledRoot = doc["ledRoot"] | 1;
        config.dayOffset = doc["dayOffset"] | 1;
        config.monthOffset = doc["monthOffset"] | 1;
        config.weekdayOffset = doc["weekdayOffset"] | 1;

        strlcpy(config.language,
                doc["language"] | "en",
                sizeof(config.language));

        config.ledRoot--;
        config.dayOffset--;
        config.monthOffset--;
        config.weekdayOffset--;

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
#if defined(ESP8266)
        File sourcefile = LittleFS.open("data.json", "r");
#elif defined(ESP32)
        File sourcefile = SPIFFS.open("/data.json", "r");
#endif

        // Allocate a temporary JsonDocument
        // Don't forget to change the capacity to match your requirements.
        // Use arduinojson.org/v6/assistant to compute the capacity.
        StaticJsonDocument<2048> doc;

        // Deserialize the JSON document
        DeserializationError error = deserializeJson(doc, sourcefile);
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
        Serial.println("Loaded configuration file from flash.");
}

void Config::save()
{
        // Delete existing file, otherwise the configuration is appended to the file
#if defined(ESP8266)
        File targetfile = LittleFS.open("data.json", "w");
#elif defined(ESP32)
        File targetfile = SPIFFS.open("/data.json", "w");
#endif

        // Open file for writing
        if (!targetfile)
        {
                Serial.println(F("Failed to create file"));
                return;
        }

        StaticJsonDocument<2048> doc;
        doc = Config::configToJSON();
        // Serialize JSON to file
        if (serializeJson(doc, targetfile) == 0)
        {
                Serial.println(F("Failed to write to file"));
        }

        // Close the file
        targetfile.close();
        Serial.println("Saved configuration file to flash.");
}
