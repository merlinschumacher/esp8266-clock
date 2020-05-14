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
Timezone localTime;
Webserver webserver;
#if defined(ESP8266)
NeoPixelBus<NeoGrbwFeature, NeoEsp8266BitBangWs2812xMethod> *strip = NULL;
#elif defined(ESP32)
NeoPixelBus<NeoGrbwFeature, NeoEsp32BitBangWs2812xMethod> *strip = NULL;
#endif

void initStrip()
{
  if (strip != NULL)
  {
    delete strip;
  }
  Serial.println(config.config.ledCount);
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
  Serial.print("Hostname: ");
  Serial.println(hostname);

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
  localTime.setLocation(config.config.timezone);
  webserver.setup(config);
  MDNS.begin(hostname.c_str());
  MDNS.addService("http", "tcp", 80);
  initStrip();
}

uint8_t calculateSecondHand()
{
  uint8_t secondHand = round((float)config.config.ledCount / 60 * localTime.second());
  // secondHand = (secondHand + config.config.ledRoot) % config.config.ledCount;
  return secondHand;
}

uint8_t calculateMinuteHand()
{
  uint8_t minuteHand = round((float)config.config.ledCount / 60 * localTime.minute());
  // minuteHand = (minuteHand + config.config.ledRoot) % config.config.ledCount;
  return minuteHand;
}

uint8_t calculateHourHand()
{
  int minuteAddition = floor((float)((config.config.ledCount / 12) / 60) * localTime.minute());
  uint8_t hourHand = floor((float)(config.config.ledCount / 12) * (localTime.hour() / 2) + minuteAddition);
  // hourHand = (hourHand + config.config.ledRoot) % config.config.ledCount;
  return hourHand;
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
    webserver.currentTime = localTime.dateTime();
    strip->ClearTo(off);

    // calculate hour dots
    float step = (float)config.config.ledCount / 12;
    for (size_t i = 1; i < 13; i++)
    {
      strip->SetPixelColor(floor(step * i), dot);
    }

    strip->SetPixelColor(calculateHourHand(), hour);
    strip->SetPixelColor(calculateMinuteHand(), minute);
    strip->SetPixelColor(calculateSecondHand(), second);

    strip->Show();
  };
  delay(100);
}