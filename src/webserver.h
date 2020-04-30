#ifndef webserver_h
#define webserver_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include "config.h"

class Webserver
{
public:
    Webserver();
    void setup(Config &config);
    void handleRequest();

private:
    void _handleNotFound();
    bool _handleFileRead(String path);
    void _handleDataGet(Config &config);
    void _handleDataPut(Config &config);
    String _getContentType(String filename);
};

#endif