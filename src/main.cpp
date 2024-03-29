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
#include "mqtt.hpp"
#include "vars.hpp"
#include "timefunc.hpp"
#include "color.hpp"
#include "led.hpp"

#if defined(ESP8266)
extern "C"
{
#include <cont.h>
  extern cont_t *g_pcont;
  void DebugFreeStack()
  {
    register uint32_t *sp asm("a1");
    int freestack = 4 * (sp - g_pcont->stack);
    Serial.printf("current free stack = %d\n", freestack);
  }
}
#endif

void renderSecondsHand(int s)
{
  uint8_t secondsHand = (s + config.config.ledRoot) % 60;
  if (config.config.fluidMotion)
  {
    HsbColor currentPixelColor, upcomingPixelColor;
    currentPixelColor = upcomingPixelColor = HsbColor(secondColor);
    float maxBrightness = currentPixelColor.B;
    float brightnessStep = maxBrightness / 60;
    float brightness = min(float(frame * brightnessStep), (float)maxBrightness);
    currentPixelColor.B = maxBrightness - brightness;
    upcomingPixelColor.B = brightness;

    uint8_t nextPixel = (secondsHand + 1) % 60;
    setPixel(secondsHand, currentPixelColor, config.config.blendColors);
    setPixel(nextPixel, upcomingPixelColor, config.config.blendColors);
  }
  else
  {
    setPixel(secondsHand, secondColor, config.config.blendColors);
  }
}

uint8_t calculateMinuteHand(int m)
{
  uint8_t minuteHand = m;
  minuteHand = (minuteHand + config.config.ledRoot) % 60;
  return minuteHand;
}

void renderHourHand(int h, int m)
{
  uint8_t hour = h % 12;
  uint8_t hourHand = floor((float)60 / 12 * hour);
  uint8_t minuteOffset = floor((float)m * 60 / 12 / 60);
  hourHand = (hourHand + minuteOffset + config.config.ledRoot) % 60;
  int8_t nextPixel = (hourHand + 1) % 60;
  int8_t prevPixel = (hourHand - 1) % 60;

  if (prevPixel == -1)
  {
    prevPixel = 59;
  }

  if (strcmp(config.config.hourHandStyle, "split") == 0)
  {
    setPixel(prevPixel, hourColor, config.config.blendColors);
    setPixel(nextPixel, hourColor, config.config.blendColors);
  }
  else if (strcmp(config.config.hourHandStyle, "wide") == 0)
  {
    RgbColor dim = hourColor.Dim(32);
    setPixel(prevPixel, dim, config.config.blendColors);
    setPixel(hourHand, hourColor, config.config.blendColors);
    setPixel(nextPixel, dim, config.config.blendColors);
  }
  else
  {
    setPixel(hourHand, hourColor, config.config.blendColors);
  }
}

uint8_t calculateDayHand()
{
  return config.config.dayOffset + day() - 1;
}

uint8_t calculateMonthHand()
{
  return config.config.monthOffset + month() - 1;
}

uint8_t calculateWeekdayHand()
{
  // correct weekday to be using 0 for monday and 7 for sunday
  uint8_t dow = localTime.weekday();
  dow--;
  if (dow == 0)
  {
    dow = 7;
  }
  return config.config.weekdayOffset + dow - 1;
}

void renderHourDots()
{
  float step = (float)60 / 12;
  for (size_t i = 0; i < 12; i++)
  {
    uint8_t dotPos = (uint8_t)floor(step * i);
    dotPos = (dotPos + config.config.ledRoot) % 60;
    if (i % 3 == 0 && config.config.hourQuarter)
    {
      strip->SetPixelColor(dotPos, quarter);
    }
    else if (config.config.hourDot)
    {
      strip->SetPixelColor(dotPos, dot);
    }
  }
}

void renderHourSegment(uint8_t h)
{

  uint8_t hour12 = h % 12;
  uint8_t segmentLength = floor((float)60 / 12);
  uint8_t segmentStart = floor((float)60 / 12 * hour12);
  segmentStart = (segmentStart + config.config.ledRoot) % 60;
  for (size_t i = 0; i < segmentLength; i++)
  {
    strip->SetPixelColor(segmentStart + i, segment);
  }
}

void setBacklight()
{
  bgStrip->ClearTo(off);
  for (size_t i = 0; i < config.config.bgLedCount; i++)
  {
    bgStrip->SetPixelColor(i, bgColor);
  }
}

void printDebugInfo()
{
#ifdef DEBUG_BUILD
  Serial.println("====");
  Serial.print("FreeHeap: ");
  Serial.println(ESP.getFreeHeap());
  DebugFreeStack();

#if defined(ESP8266)
  Serial.print("HeapFragmentation: ");
  Serial.println(ESP.getHeapFragmentation());
  Serial.print("MaxFreeBlockSize: ");
  Serial.println(ESP.getMaxFreeBlockSize());
#elif defined(ESP32)
  Serial.print(xPortGetFreeHeapSize());
#endif
  Serial.print("Uptime: ");
  Serial.println(int(millis() / 1000));
  Serial.println("====");
#endif
}

void showStrips()
{

  while (!strip->CanShow())
  {
    delay(1);
  }

  strip->Show();
  while (!bgStrip->CanShow())
  {
    delay(1);
  }
  bgStrip->Show();
}
void clearStrips()
{
  strip->ClearTo(off);
  bgStrip->ClearTo(off);
}

void shiftStrips(uint8_t frameskip = 1)
{
  if (frame % frameskip == 0)
  {
    strip->RotateRight(1);
    if (config.config.bgLight)
      bgStrip->RotateRight(1);
  }
}

void renderTime()
{
  if (config.config.hourDot)
    renderHourDots();
  if (config.config.hourSegment)
    renderHourSegment(currentHour);

  renderHourHand(currentHour, currentMinute);
  setPixel(calculateMinuteHand(currentMinute), minuteColor, config.config.blendColors);
  renderSecondsHand(currentSecond);
  if (config.config.dayMonth)
  {
    setPixel(currentDayPos, dayColor, config.config.blendColors);
    setPixel(currentMonthPos, monthColor, config.config.blendColors);
    setPixel(currentWeekdayPos, weekdayColor, config.config.blendColors);
  }
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
  SPIFFS.begin(true);
#endif
  config.load();
  initStrip();
  clearStrips();
  char hostname[64];
  char apname[68] = "";
  strlcpy(hostname, config.config.hostname, sizeof(hostname));
  snprintf(apname, sizeof(apname), "⏰%s", hostname);
  Serial.print("Hostname: ");
  Serial.println(hostname);

#if defined(ESP8266)
  WiFi.hostname(hostname);
#elif defined(ESP8266)
  WiFi.setHostname(hostname);
#endif

  wifiManager.autoConnect(apname);
#ifdef DEBUG_BUILD
  setDebug(DEBUG);
#endif
  setServer(config.config.timeserver);
  setInterval(20000);
  waitForSync(10);

  Serial.println("UTC: " + UTC.dateTime());
  localTime.setLocation(config.config.timezone);
  localTime.setDefault();
  webserver.setup(config);
  mqtt.setup(config);
  MDNS.begin(hostname);
  MDNS.addService("ESPCLOCK", "tcp", 80);
  MDNS.addService("http", "tcp", 80);
  updateColors(isNight(hour(), minute()));
}

void loop()
{
  webserver.handleRequest();
  if (webserver.triggerWifiConf)
  {
    clearStrips();
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    char apname[68] = "";
    snprintf(apname, sizeof(apname), "⏰%s", config.config.hostname);
    wifiManager.startConfigPortal(apname);
  }

  if (!config.locked)
  {

    uint8_t s = second();

    if (config.tainted)
    {
      alarm = isAlarm();
      uint8_t m = minute();
      uint8_t h = hour();
      night = isNight(h, m);
      updateColors(night);
      mqtt.connect(config);
      mqtt.publishConfig(config);
      config.tainted = false;
    };

    if (currentSecond != s)
    {
      currentSecond = s;
      alarm = isAlarm();
      frame = 0;
      uint8_t m = minute();
      mqtt.connect(config);
#ifdef DEBUG_BUILD
      mqtt.publishUptime();
#endif

      if (currentMinute != m)
      {
        currentMinute = m;
        uint8_t h = hour();

        night = isNight(h, m);
        topHour = (config.config.hourLight && currentMinute == 0);
        updateColors(night);
        printDebugInfo();

        if (currentHour != h)
        {
          currentHour = h;
          currentDayPos = calculateDayHand();
          currentMonthPos = calculateMonthHand();
          currentWeekdayPos = calculateWeekdayHand();
        }
      }
    }

    if (tick())
    {
      if (alarm || strcmp(mqtt.currentCommand, "alarm") == 0)
      {
        if (!animationRendered)
        {
          renderAlarm(night);
          if (config.config.bgLight)
            renderAlarm(night, true);
          animationRendered = true;
          showStrips();
          mqtt.publishStatus("alarm");
          return;
        }
        shiftStrips(2);
        showStrips();
      }

      else if (topHour || strcmp(mqtt.currentCommand, "rainbow") == 0)
      {
        if (!animationRendered)
        {
          renderRainbow(night);
          if (config.config.bgLight)
            renderRainbow(night, true);
          animationRendered = true;
          showStrips();
          mqtt.publishStatus("rainbow");
          return;
        }
        shiftStrips(2);
        showStrips();
      }
      else if (strcmp(mqtt.currentCommand, "off") == 0)
      {
        clearStrips();
        showStrips();
        mqtt.publishStatus("off");
      }

      else
      {
        animationRendered = false;
        clearStrips();
        renderTime();
        setBacklight();
        showStrips();
        mqtt.publishStatus("time");
      }
    }
  }
  mqtt.loop();
  MDNS.update();
  events();
}