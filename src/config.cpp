#include <Arduino.h>
#include "config.h"

Config::Config()
{
}

StaticJsonDocument<512> Config::configToJSON()
{
    StaticJsonDocument<512> doc;
    // Set the values in the document
    doc["hostname"] = config.hostname;
    doc["timezone"] = config.timezone;
    doc["hourColor"] = config.hourColor;
    doc["minuteColor"] = config.minuteColor;
    doc["secondColor"] = config.secondColor;

    doc["brightnessDay"] = config.brightnessDay;
    doc["brightnessDimmed"] = config.brightnessDimmed;

    doc["timeserver"] = config.timeserver;

    return doc;
}

void Config::JSONToConfig(StaticJsonDocument<512> doc)
{
    config.hourColor = doc["hourColor"] | 16777215;
    config.minuteColor = doc["minuteColor"] | 16777215;
    config.secondColor = doc["secondColor"] | 16777215;

    config.brightnessDay = doc["brightnessDay"] | 255;
    config.brightnessDimmed = doc["brightnessDimmed"] | 128;

    char chipid[8];
    String hostname = "ESPCLOCK-";
    if (not doc.containsKey("hostname"))
    {
        String(ESP.getChipId()).toCharArray(chipid, 8);
        hostname += String(chipid);
    }

    strlcpy(config.timezone,                    // <- destination
            doc["timezone"] | "Europe/Berlin",  // <- source
            sizeof(config.timeserver));         // <- destination's capacity
    strlcpy(config.hostname,                    // <- destination
            doc["hostname"] | hostname.c_str(), // <- source
            sizeof(config.timeserver));         // <- destination's capacity
    strlcpy(config.timeserver,                  // <- destination
            doc["timeserver"] | "pool.ntp.org", // <- source
            sizeof(config.timeserver));         // <- destination's capacity
}

void Config::load()
{
    Serial.println("Load file");
    // Open file for reading
    File sourcefile = SPIFFS.open("data.json", "r+");
    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<512> doc;

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
}

void Config::save()
{
    // Delete existing file, otherwise the configuration is appended to the file
    File targetfile = SPIFFS.open("data.json", "w+");

    // Open file for writing
    if (!targetfile)
    {
        Serial.println(F("Failed to create file"));
        return;
    }
    Serial.println("file open complete");

    StaticJsonDocument<512> doc;
    Serial.println("created doc");
    doc = Config::configToJSON();
    Serial.println("config to Json done");
    serializeJson(doc, Serial);
    // Serialize JSON to file
    if (serializeJson(doc, targetfile) == 0)
    {
        Serial.println(F("Failed to write to file"));
    }
    Serial.println("wrote file");

    // Close the file
    targetfile.close();
    Serial.println("closed file");
}
