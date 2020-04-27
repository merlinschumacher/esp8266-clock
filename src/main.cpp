#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include "webserver.h"
#include "config.h"

Config config;
Webserver webserver(&config);
void setup()
{
  Serial.begin(115200);
  WiFiManager wifiManager;
  String chipid = "ESP_CLOCK-" + String(ESP.getChipId()); // The chip ID is essentially its MAC address(length: 6 bytes).
  WiFi.hostname(chipid.c_str());
  wifiManager.autoConnect(chipid.c_str());
  Serial.println("Connected to WiFi");
  Serial.println("Loading config");
  config.load();
  Serial.println("starting webserver");
  webserver.setup();
}

void loop()
{
  webserver.handleRequest();
  MDNS.update();
  // put your main code here, to run repeatedly:
}