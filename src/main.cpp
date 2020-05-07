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
#if defined(ESP8266)
NeoPixelBus<NeoGrbwFeature, NeoEsp8266BitBangWs2812xMethod> *strip = NULL;
#elif defined(ESP32)
NeoPixelBus<NeoGrbwFeature, NeoEsp32BitBangWs2812xMethod> *strip = NULL;
#endif
Timezone localTime;

void initStrip()
{
  if (strip != NULL)
  {
    delete strip;
  }
#if defined(ESP8266)
  strip = new NeoPixelBus<NeoGrbwFeature, NeoEsp8266BitBangWs2812xMethod>(config.config.ledCount, config.config.ledPin);
#elif defined(ESP32)
  strip = new NeoPixelBus<NeoGrbwFeature, NeoEsp32BitBangWs2812xMethod>(config.config.ledCount, config.config.ledPin);
#endif
  strip->Begin();
}

void setup()
{
  Serial.begin(115200);
  WiFiManager wifiManager;
#if defined(ESP8266)
  SPIFFS.begin();
#elif defined(ESP32)
  SPIFFS.begin();
#endif
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
  uint8_t secondHand = round((float)config.config.ledCount / 60 * localTime.second());
  return secondHand;
}

uint8_t calculateMinuteHand()
{
  uint8_t minuteHand = round((float)config.config.ledCount / 60 * localTime.minute());
  return minuteHand;
}

uint8_t calculateHourHand()
{
  int minuteAddition = floor((float)((config.config.ledCount / 12) / 60) * localTime.minute());
  uint8_t hourHand = floor((float)(config.config.ledCount / 12) * (localTime.hour() / 2) + minuteAddition);
  return hourHand;
}

uint8_t *calculateHourDots()
{
  static uint8_t dots[12];
  for (size_t i = 1; i < 13; i++)
  {
    dots[i] = floor((float)(config.config.ledCount / 12 * i));
  }

  return dots;
}

void loop()
{
#if defined(ESP8266)
  MDNS.update();
#endif
  webserver.handleRequest();
  RgbwColor off(0, 0, 0, 0);
  HtmlColor hour, minute, second, dot;
  hour.Parse<HtmlShortColorNames>(config.config.hourColor, sizeof(config.config.hourColor));
  minute.Parse<HtmlShortColorNames>(config.config.minuteColor, sizeof(config.config.minuteColor));
  second.Parse<HtmlShortColorNames>(config.config.secondColor, sizeof(config.config.secondColor));
  dot.Parse<HtmlShortColorNames>(config.config.hourDotColor, sizeof(config.config.hourDotColor));

  if (strip != NULL && secondChanged())
  {
    // Serial.println("Current time: " + localTime.dateTime());
    strip->ClearTo(off);
    strip->SetPixelColor(calculateSecondHand(), second);
    strip->SetPixelColor(calculateMinuteHand(), minute);
    strip->SetPixelColor(calculateHourHand(), hour);

    uint8_t *dots = calculateHourDots();
    for (size_t i = 0; i < sizeof(dots); i++)
    {
      Serial.println(dots[i]);
      strip->SetPixelColor(dots[i], dot);
    }

    strip->Show();
  }
  delay(100);
}