#ifndef mqtt_h
#define mqtt_h
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FS.h>
#include <LittleFS.h>
#include <ESP8266HTTPClient.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <SPIFFS.h>
#endif

#include <PubSubClient.h>
#include "config.hpp"

class Mqtt
{

public:
    Mqtt();
    bool setup(Config &config);
    void publish(const char *message, const char *topic, bool retain = false);
    bool connect(Config &config);
    void publishConfig(Config &config);
    void publishStatus(const char *status);
#ifdef DEBUG_BUILD
    void publishUptime();
#endif
    void loop();
    char statusTopic[255] = {0};
    char commandTopic[255] = {0};
    char configTopic[255] = {0};
#ifdef DEBUG_BUILD
    char debugTopic[255] = {0};
#endif
    char setConfigTopic[255] = {0};
    char currentCommand[32];

private:
    void _handleRequest(char *topic, byte *payload, unsigned int length, Config &config);
    WiFiClient _wifiClient = WiFiClient();
    WiFiClientSecure _wifiClientSecure = WiFiClientSecure();
    PubSubClient _mqttClient = PubSubClient();
    char _lastStatus[32] = {0};
    bool _isEnabled = false;
};

#endif // mqtt_h
