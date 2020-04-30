#define EZTIME_CACHE_EEPROM

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>
#include <ezTime.h>
#include "webserver.h"
#include "config.h"

Config config;
Webserver webserver;
void setup()
{
  Serial.begin(115200);
  WiFiManager wifiManager;
  SPIFFS.begin();
  char chipid[8];
  String(ESP.getChipId()).toCharArray(chipid, 8);
  String apname = "⏰ESPCLOCK-" + String(chipid);
  String hostname = "ESPCLOCK-" + String(chipid);
  Serial.println("Loading config");
  config.load();
  strlcpy(config.config.hostname, hostname.c_str(), sizeof(config.config.hostname));
  WiFi.hostname(hostname.c_str());
  wifiManager.autoConnect(apname.c_str());
  Serial.println("Connected to WiFi");
  waitForSync();
  Timezone localtime;
  localtime.setLocation(String(config.config.timezone));
  Serial.println("UTC time: " + UTC.dateTime());
  Serial.println("Local time: " + localtime.dateTime());
  Serial.print("Hostname: ");
  Serial.println(hostname);
  Serial.println("starting webserver");
  webserver.setup(config);
  MDNS.begin(hostname);
  MDNS.addService("http", "tcp", 80);
}

void loop()
{
  webserver.handleRequest();
  MDNS.update();
  // put your main code here, to run repeatedly:
}