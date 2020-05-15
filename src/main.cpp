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
NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBangWs2812xMethod> *strip = NULL;
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
  strip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBangWs2812xMethod>(config.config.ledCount, config.config.ledPin);
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
  uint8_t secondHand = round((float)(config.config.ledCount + 1) / 60 * localTime.second());
  secondHand = (secondHand + config.config.ledRoot) % config.config.ledCount;
  return secondHand;
}

uint8_t calculateMinuteHand()
{
  uint8_t minuteHand = round((float)(config.config.ledCount + 1) / 60 * localTime.minute());
  minuteHand = (minuteHand + config.config.ledRoot) % config.config.ledCount;
  return minuteHand;
}

uint8_t calculateHourHand()
{
  uint8_t hour = localTime.hour() % 12;
  uint8_t hourHand = floor((float)(config.config.ledCount + 1) / 12 * hour);
  uint8_t minuteBracket = floor((float)localTime.minute() / 12.5);
  uint8_t minuteOffset = floor((float)(config.config.ledCount + 1) / 60 * minuteBracket);
  hourHand = (hourHand + minuteOffset + config.config.ledRoot) % config.config.ledCount;
  return hourHand;
}

void setPixel(uint8_t pos, RgbColor color, bool blend = true)
{
  if (blend)
  {
    RgbColor currentcolor = strip->GetPixelColor(pos);
    uint8_t colorSum = currentcolor.R + currentcolor.G + currentcolor.B;
    if (colorSum == 0)
    {
      strip->SetPixelColor(pos, color);
    }
    else
    {
      RgbColor targetcolor = RgbColor::LinearBlend(currentcolor, color, 0.25);
      strip->SetPixelColor(pos, targetcolor);
    }
  }
  else
  {
    strip->SetPixelColor(pos, color);
  }
}

void loop()
{
#if defined(ESP8266)
  MDNS.update();
#endif
  webserver.handleRequest();

  HtmlColor htmlconv;
  htmlconv.Parse<HtmlShortColorNames>("#000000");
  RgbColor off(htmlconv);
  htmlconv.Parse<HtmlShortColorNames>(config.config.hourColor, sizeof(config.config.hourColor));
  RgbColor hour(htmlconv);
  htmlconv.Parse<HtmlShortColorNames>(config.config.minuteColor, sizeof(config.config.minuteColor));
  RgbColor minute(htmlconv);
  htmlconv.Parse<HtmlShortColorNames>(config.config.secondColor, sizeof(config.config.secondColor));
  RgbColor second(htmlconv);
  htmlconv.Parse<HtmlShortColorNames>(config.config.hourDotColor, sizeof(config.config.hourDotColor));
  RgbColor dot(htmlconv);
  htmlconv.Parse<HtmlShortColorNames>(config.config.hourQuarterColor, sizeof(config.config.hourQuarterColor));
  RgbColor quarter(htmlconv);
  htmlconv.Parse<HtmlShortColorNames>(config.config.hourSegmentColor, sizeof(config.config.hourSegmentColor));
  RgbColor segment(htmlconv);

  if (strip != NULL && secondChanged())
  {
    webserver.currentTime = localTime.dateTime();
    strip->ClearTo(off);

    uint8_t hour12 = localTime.hour() % 12;
    uint8_t segmentLength = floor((float)(config.config.ledCount + 1) / 12);
    uint8_t segmentStart = floor((float)(config.config.ledCount + 1) / 12 * hour12);
    segmentStart = (segmentStart + config.config.ledRoot) % config.config.ledCount;
    for (size_t i = 0; i < segmentLength; i++)
    {
      strip->SetPixelColor(segmentStart + i, segment);
    }

    // calculate hour dots
    float step = (float)(config.config.ledCount + 1) / 12;
    for (size_t i = 0; i < 12; i++)
    {
      uint8_t dotPos = (uint8_t)floor(step * i);
      dotPos = (dotPos + config.config.ledRoot) % config.config.ledCount;
      if (i % 3 == 0)
      {
        strip->SetPixelColor(dotPos, quarter);
      }
      else
      {
        strip->SetPixelColor(dotPos, dot);
      }
    }

    setPixel(calculateHourHand(), hour);
    setPixel(calculateMinuteHand(), minute);
    setPixel(calculateSecondHand(), second);

    strip->Show();
  };
}