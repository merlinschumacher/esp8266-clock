#include <Arduino.h>
#include "config.h"

Config::Config()
{
    Serial.begin(115200);
    Serial.print("Config init");
}

StaticJsonDocument<512> Config::configToJSON()
{
    StaticJsonDocument<512> doc;
    // Set the values in the document
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
    config.timezone = doc["timezone"] | 0;
    config.hourColor = doc["hourColor"] | 16777215;
    config.minuteColor = doc["minuteColor"] | 16777215;
    config.secondColor = doc["secondColor"] | 16777215;

    config.brightnessDay = doc["brightnessDay"] | 255;
    config.brightnessDimmed = doc["brightnessDimmed"] | 128;

    strlcpy(config.timeserver,                  // <- destination
            doc["timeserver"] | "pool.ntp.org", // <- source
            sizeof(config.timeserver));         // <- destination's capacity
}

void Config::load()
{
    Serial.println("Load file");
    // Open file for reading
    File file = _filesystem->open("data.json", "r");
    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<512> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);

    Config::JSONToConfig(doc);
    if (error)
    {
        Serial.println(F("Failed to read file, using default configuration"));
        Config::save();
    }
    // Copy values from the JsonDocument to the Config
    // Close the file (Curiously, File's destructor doesn't close the file)
    file.close();
}

void Config::save()
{
    // Delete existing file, otherwise the configuration is appended to the file
    File file = _filesystem->open("data.json", "w");

    // Open file for writing
    if (!file)
    {
        Serial.println(F("Failed to create file"));
        return;
    }

    StaticJsonDocument<512> doc;
    doc = Config::configToJSON();
    // Serialize JSON to file
    if (serializeJson(doc, file) == 0)
    {
        Serial.println(F("Failed to write to file"));
    }

    // Close the file
    file.close();
}
