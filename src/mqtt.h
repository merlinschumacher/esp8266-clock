#ifndef mqtt_h
#define mqtt_h
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#define STATUS
#define CONFIG
#define EFFECT

class Mqtt
{

public:
    Mqtt();
    void handleRequest(char *topic, byte *payload, unsigned int length);
    void connect();
    void publish(String message, uint8_t type);
    char server[64];
    char username[64];
    char password[64];
    char clientID[64];
    char baseTopic[128];
    uint16_t port = 1883;

private:
    uint64_t _lastMillis = 0;
    char _statusTopic[128];
    char _configTopic[128];
    char _effectTopic[128];
};

#endif // mqtt_h