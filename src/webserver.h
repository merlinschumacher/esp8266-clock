#ifndef webserver_h
#define webserver_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <FS.h>
#include "config.h"

class Webserver
{
public:
    Webserver(Config *config);
    void setup();
    void handleRequest();

private:
    void _handleNotFound();
    bool _handleFileRead(String path);
    bool _handleDataGet();
    void _handleForm();
    String _getContentType(String filename);
    FS *_filesystem = &SPIFFS;
    Config *_config;
};

#endif