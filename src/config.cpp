#include <Arduino.h>
#include "config.h"

Config::Config()
{
}

StaticJsonDocument<1024> Config::configToJSON()
{
        StaticJsonDocument<1024> doc;
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

        doc["nightTimeBegins"] = config.nightTimeBegins;
        doc["nightTimeEnds"] = config.nightTimeEnds;

        doc["hourLight"] = config.hourLight;

        doc["ledPin"] = config.ledPin;
        doc["ledCount"] = config.ledCount + 1;
        doc["ledRoot"] = config.ledRoot + 1;

        return doc;
}

bool Config::JSONToConfig(StaticJsonDocument<1024> doc)
{
        char chipid[8];
        String hostname = "ESPCLOCK-";
        if (not doc.containsKey("hostname"))
        {
#if defined(ESP8266)
                String(ESP.getChipId()).toCharArray(chipid, 8);
#elif defined(ESP32)
                String((uint32_t)ESP.getEfuseMac()).toCharArray(chipid, 8);
#endif
                hostname += String(chipid);
        }

        strlcpy(config.hostname,
                doc["hostname"] | hostname.c_str(),
                sizeof(config.hostname));
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
                doc["minuteColor"] | "#007700",
                sizeof(config.minuteColorDimmed));
        strlcpy(config.secondColorDimmed,
                doc["secondColor"] | "#000077",
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

        strlcpy(config.nightTimeBegins,
                doc["nightTimeBegins"] | "22:00",
                sizeof(config.nightTimeBegins));
        strlcpy(config.nightTimeEnds,
                doc["nightTimeEnds"] | "08:00",
                sizeof(config.nightTimeEnds));

        config.hourLight = doc["hourLight"] | false;

        config.ledPin = doc["ledPin"] | 4;
        config.ledCount = doc["ledCount"] | 60;
        config.ledRoot = doc["ledRoot"] | 1;
        config.ledCount--;
        config.ledRoot--;

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
        File sourcefile = SPIFFS.open("data.json", "r");
#elif defined(ESP32)
        File sourcefile = SPIFFS.open("data.json", "r");
#endif

        // Allocate a temporary JsonDocument
        // Don't forget to change the capacity to match your requirements.
        // Use arduinojson.org/v6/assistant to compute the capacity.
        StaticJsonDocument<1024> doc;

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
        File targetfile = SPIFFS.open("data.json", "w");
#elif defined(ESP32)
        File targetfile = SPIFFS.open("data.json", "w");
#endif

        // Open file for writing
        if (!targetfile)
        {
                Serial.println(F("Failed to create file"));
                return;
        }

        StaticJsonDocument<1024> doc;
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
