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
#include "webserver.h"
#include "config.h"
#include "vars.h"
#include "time.h"
#include "color.h"
#include "led.h"

void renderSecondHand()
{
  uint8_t secondHand = floor((float)(config.config.ledCount / 60) * localTime.second());
  secondHand = (secondHand + config.config.ledRoot) % config.config.ledCount;
  if (config.config.fluidMotion)
  {
    RgbColor currentPixelColor, upcomingPixelColor;
    float percent = (ms() % 1000) / 10;
    currentPixelColor = DimColor(percent, secondColor);
    upcomingPixelColor = DimColor(100 - percent, secondColor);

    setPixel(secondHand, currentPixelColor, false);
    setPixel(secondHand + 1, upcomingPixelColor, false);
  }
  else
  {
    setPixel(secondHand, secondColor, config.config.blendColors);
  }
}

uint8_t calculateMinuteHand()
{
  uint8_t minuteHand = floor((float)(config.config.ledCount / 60) * localTime.minute());
  minuteHand = (minuteHand + config.config.ledRoot) % config.config.ledCount;
  return minuteHand;
}

uint8_t calculateHourHand()
{
  uint8_t hour = localTime.hour() % 12;
  uint8_t hourHand = floor((float)config.config.ledCount / 12 * hour);
  uint8_t minuteOffset = floor((float)localTime.minute() * config.config.ledCount / 12 / 60);
  hourHand = (hourHand + minuteOffset + config.config.ledRoot) % config.config.ledCount;
  return hourHand;
}

void showClockElements()
{

  webserver.currentTime = localTime.dateTime();
  strip->ClearTo(off);

  uint8_t hour12 = localTime.hour() % 12;
  uint8_t segmentLength = floor((float)config.config.ledCount / 12);
  uint8_t segmentStart = floor((float)config.config.ledCount / 12 * hour12);
  segmentStart = (segmentStart + config.config.ledRoot) % config.config.ledCount;
  for (size_t i = 0; i < segmentLength; i++)
  {
    strip->SetPixelColor(segmentStart + i, segment);
  }

  // calculate hour dots
  float step = (float)config.config.ledCount / 12;
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

  setPixel(calculateHourHand(), hourColor, config.config.blendColors);
  setPixel(calculateMinuteHand(), minuteColor, config.config.blendColors);
  renderSecondHand();

  strip->Show();
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
  updateColors();
  currentMinute = minute();
  currentSecond = second();
}

void loop()
{
#if defined(ESP8266)
  MDNS.update();
#endif
  webserver.handleRequest();
  events();

  uint8_t sec = second();
  uint8_t min = minute();

  if (currentMinute != min)
  {
    currentMinute = minute();
    updateColors();
  }

  if (isAlarm() && tick())
  {
    alarmAnimation();
  }
  else if (config.config.hourLight && currentMinute == 0 && !isAlarm() && tick())
  {
    hourRainbow();
  }
  else if (!isAlarm() && currentMinute != 0)
  {
    currentSecond = second();
    showClockElements();
    animationPos = 0;
  }
}
