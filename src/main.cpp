#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#endif
#include <WiFiManager.h>
#include <NeoPixelBus.h>
#include <ezTime.h>
#include <NeoPixelAnimator.h>
#include "webserver.h"
#include "config.h"

Config config;
Webserver webserver;
NeoPixelBus<NeoGrbwFeature, NeoEsp8266BitBangWs2812xMethod> *strip = NULL;
Timezone localTime;

void initStrip()
{
  if (strip != NULL)
  {
    delete strip;
  }
  strip = new NeoPixelBus<NeoGrbwFeature, NeoEsp8266BitBangWs2812xMethod>(config.config.ledCount, config.config.ledPin);
  strip->Begin();
}

void setup()
{
  Serial.begin(115200);
  WiFiManager wifiManager;
  SPIFFS.begin();
  config.load();
  String hostname = config.config.hostname;
  String apname = "⏰" + hostname;

#if defined(ESP8266)
  WiFi.hostname(hostname.c_str());
#elif defined(ESP8266)
  WiFi.setHostname(hostname.c_str());
#endif

  wifiManager.autoConnect(apname.c_str());
  setServer(config.config.timeserver);
  waitForSync();
  setDebug(INFO);
  Serial.println("UTC: " + UTC.dateTime());
  webserver.setup(config);
  MDNS.begin(hostname.c_str());
  MDNS.addService("http", "tcp", 80);
  initStrip();
  Serial.print("Hostname: ");
  Serial.println(hostname);
  localTime.setLocation(config.config.timezone);
}

uint8_t calculateSecondHand()
{
  return (uint8_t)round((float)config.config.ledCount / 60 * localTime.second());
}

uint8_t calculateMinuteHand()
{
  return (uint8_t)round((float)config.config.ledCount / 60 * localTime.minute());
}

uint8_t calculateHourHand()
{
  int minuteAddition = floor((float)((config.config.ledCount / 12) / 60) * localTime.minute());
  return (uint8_t)round(((float)config.config.ledCount / 12) * (localTime.hour() / 2) + minuteAddition);
}

void loop()
{
#if defined(ESP8266)
  MDNS.update();
#endif
  webserver.handleRequest();
  RgbwColor red(128, 0, 0, 0);
  RgbwColor off(0, 0, 0, 0);
  RgbwColor green(0, 128, 0, 0);
  RgbwColor blue(0, 0, 128, 0);
  HtmlColor color;
  color.Parse<HtmlShortColorNames>(config.config.hourColor, sizeof(config.config.hourColor));

  if (strip != NULL && secondChanged())
  {
    // Serial.println("Current time: " + localTime.dateTime());
    strip->ClearTo(off);
    strip->SetPixelColor(calculateSecondHand(), red);
    strip->SetPixelColor(calculateMinuteHand(), green);
    strip->SetPixelColor(calculateHourHand(), color);
    strip->Show();
  }
}