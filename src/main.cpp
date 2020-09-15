#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <LittleFS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#endif
#include <WiFiManager.h>
#include <NeoPixelBus.h>
#include <ezTime.h>
#include "webserver.hpp"
#include "config.hpp"
#include "vars.hpp"
#include "timefunc.hpp"
#include "color.hpp"
#include "led.hpp"

void renderSecondHand()
{
  uint8_t secondHand = floor((float)(config.config.ledCount / 60) * localTime.second());
  secondHand = (secondHand + config.config.ledRoot) % config.config.ledCount;
  if (config.config.fluidMotion)
  {
    RgbColor currentPixelColor, upcomingPixelColor;
    float percent = (localTime.ms() % 1000) / 10;
    currentPixelColor = DimColor(percent, secondColor);
    upcomingPixelColor = DimColor(100 - percent, secondColor);

    uint8_t nextPixel = (secondHand + 1) % 60;
    setPixel(secondHand, currentPixelColor, config.config.blendColors);
    setPixel(nextPixel, upcomingPixelColor, config.config.blendColors);
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

uint8_t calculateDayHand()
{
  return config.config.ledDayRoot + localTime.day();
}

uint8_t calculateMonthHand()
{
  return config.config.ledMonthRoot + localTime.month();
}

uint8_t calculateWeekdayHand()
{
  return config.config.ledWeekdayRoot + localTime.weekday();
}

void renderHourDots()
{
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
}

void renderHourSegment()
{

  uint8_t hour12 = localTime.hour() % 12;
  uint8_t segmentLength = floor((float)config.config.ledCount / 12);
  uint8_t segmentStart = floor((float)config.config.ledCount / 12 * hour12);
  segmentStart = (segmentStart + config.config.ledRoot) % config.config.ledCount;
  for (size_t i = 0; i < segmentLength; i++)
  {
    strip->SetPixelColor(segmentStart + i, segment);
  }
}

void setBacklight()
{
  if (config.config.bgLight && !isAlarm() && currentMinute != 0)
  {
    for (size_t i = 0; i < config.config.bgLedCount; i++)
    {
      bgStrip->SetPixelColor(i, bgColor);
    }
    bgStrip->Show();
  }
}

void printDebugInfo()
{
#ifdef DEBUG_BUILD
  Serial.println("====");
  Serial.print("FreeHeap: ");
  Serial.println(ESP.getFreeHeap());
#if defined(ESP8266)
  Serial.print("HeapFragmentation: ");
  Serial.println(ESP.getHeapFragmentation());
  Serial.print("MaxFreeBlockSize: ");
  Serial.println(ESP.getMaxFreeBlockSize());
#else if defined(ESP32)
  Serial.print(xPortGetFreeHeapSize());
#endif
  Serial.print("Uptime: ");
  Serial.println(int(millis() / 1000));
  Serial.println("====");
#endif
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.print("ESPCLOCK Version ");
  Serial.println(VERSION);
  WiFiManager wifiManager;
#if defined(ESP8266)
  LittleFS.begin();
#elif defined(ESP32)
  SPIFFS.format();
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
#ifdef DEBUG_BUILD
  setDebug(DEBUG);
#endif
  setServer(config.config.timeserver);
  waitForSync();

  Serial.println("UTC: " + UTC.dateTime());
  localTime.setLocation(config.config.timezone);
  localTime.setDefault();
  webserver.setup(config);
  MDNS.begin(hostname.c_str());
  MDNS.addService("http", "tcp", 80);
  initStrip();
  updateColors();
  currentMinute = minute();
  currentSecond = second();
  webserver.version = VERSION;
}

void loop()
{
#if defined(ESP8266)
  MDNS.update();
#endif
  webserver.handleRequest();
  if (timeStatus() != timeSet)
  {
    return;
  }
  events();

  uint8_t sec = second();

  strip->ClearTo(off);
  if (currentSecond != sec)
  {
    currentMinute = minute();
    currentSecond = sec;
    updateColors(isNight());
    webserver.currentTime = localTime.dateTime();
    setBacklight();
    printDebugInfo();
  }

  if (isAlarm() && tick())
  {
    alarmAnimation(isNight());
  }
  else if (config.config.hourLight && currentMinute == 0 && tick())
  {
    hourRainbow(isNight());
  }
  else if (tick())
  {
    currentSecond = sec;
    renderHourDots();
    renderHourSegment();

    setPixel(calculateHourHand(), hourColor, config.config.blendColors);
    setPixel(calculateMinuteHand(), minuteColor, config.config.blendColors);
    renderSecondHand();

    strip->Show();
    animationPos = 0;
  }
  if (minuteChanged())
  {
    Serial.println(calculateDayHand());
    Serial.println(calculateMonthHand());
    Serial.println(calculateWeekdayHand());
  }

  yield();
}
