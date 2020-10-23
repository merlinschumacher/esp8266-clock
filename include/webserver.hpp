#ifndef webserver_h
#define webserver_h

#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
#include <SPIFFS.h>
#endif
#include "config.hpp"

class Webserver
{
public:
    Webserver();
    void setup(Config &config);
    void handleRequest();
    char currentTime[64];

private:
    void _handleNotFound();
    void _handleDataGet(Config &config);
    void _handleDataPut(Config &config);
    void _resetConfig(Config &config);
    void _handleIndex(Config &config);
    String _getContentType(String filename);
};

#endif //webserver_h