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
  config.load();
  String hostname = config.config.hostname;
  String apname = "⏰" + hostname;
  strlcpy(config.config.hostname, hostname.c_str(), sizeof(config.config.hostname));
  WiFi.hostname(hostname.c_str());
  wifiManager.autoConnect(apname.c_str());
  waitForSync();
  Timezone localtime;
  localtime.setLocation(String(config.config.timezone));
  Serial.println("UTC time: " + UTC.dateTime());
  Serial.println("Local time: " + localtime.dateTime());
  Serial.print("Hostname: ");
  Serial.println(hostname);
  webserver.setup(config);
  MDNS.begin(hostname);
  MDNS.addService("http", "tcp", 80);
}

void loop()
{
  webserver.handleRequest();
  MDNS.update();
}