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
NeoPixelBus<NeoGrbFeature, NeoEsp32BitBangWs2812xMethod> *strip = NULL;
#endif
RgbColor animationColor;
uint8_t animationPos;
RgbColor off(0, 0, 0);
String hourColorString, minuteColorString, secondColorString, hourDotString, hourQuarterString, hourSegmentString;
RgbColor hourColor(0, 0, 0);
RgbColor minuteColor(0, 0, 0);
RgbColor secondColor(0, 0, 0);
RgbColor dot(0, 0, 0);
RgbColor quarter(0, 0, 0);
RgbColor segment(0, 0, 0);
uint8_t currentMinute = 60;
uint8_t currentSecond = 60;

void initStrip()
{
  if (strip != NULL)
  {
    delete strip;
  }
#if defined(ESP8266)
  strip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBangWs2812xMethod>(config.config.ledCount, config.config.ledPin);
#elif defined(ESP32)
  strip = new NeoPixelBus<NeoGrbFeature, NeoEsp32BitBangWs2812xMethod>(config.config.ledCount, config.config.ledPin);
#endif
  strip->Begin();
}

uint8_t calculateSecondHand()
{
  uint8_t secondHand = floor((float)(config.config.ledCount / 60) * localTime.second());
  secondHand = (secondHand + config.config.ledRoot) % config.config.ledCount;
  return secondHand;
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

HtmlColor htmlToColor(String color)
{
  HtmlColor htmlconv;
  htmlconv.Parse<HtmlShortColorNames>(color);
  RgbColor rgb(htmlconv);
  return rgb;
}
String split(String s, char parser, int index)
{
  String rs = "";
  int parserCnt = 0;
  int rFromIndex = 0, rToIndex = -1;
  while (index >= parserCnt)
  {
    rFromIndex = rToIndex + 1;
    rToIndex = s.indexOf(parser, rFromIndex);
    if (index == parserCnt)
    {
      if (rToIndex == 0 || rToIndex == -1)
        return "";
      return s.substring(rFromIndex, rToIndex);
    }
    else
      parserCnt++;
  }
  return rs;
}

String getStringPartByNr(String data, char separator, int index)
{

  uint8_t stringData = 0;
  String dataPart = "";
  for (uint8_t i = 0; i < data.length(); i++)
  {
    if (data[i] == separator)
    {
      stringData++;
    }
    else if (stringData == index)
    {
      dataPart.concat(data[i]);
    }
    else if (stringData > index)
    {
      return dataPart;
      break;
    }
  }
  return dataPart;
}

uint16_t timeFromString(String time)
{
  uint8_t hour, minute;
  String hourString = getStringPartByNr(time, ':', 0);
  String minuteString = getStringPartByNr(time, ':', 1);
  hour = hourString.toInt();
  minute = minuteString.toInt();
  return hour * 60 + minute;
}

bool isNight()
{
  uint16_t nightStartMinutes = timeFromString(config.config.nightTimeBegins);
  uint16_t nightEndMinutes = timeFromString(config.config.nightTimeEnds);
  uint16_t currentMinutes = localTime.hour() * 60 + localTime.minute();
  if (nightStartMinutes > nightEndMinutes)
  {
    if (nightStartMinutes <= currentMinutes || currentMinutes <= nightEndMinutes)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    if (nightStartMinutes <= currentMinutes && currentMinutes <= nightEndMinutes)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

bool isAlarm()
{
  uint16_t alarmMinutes = timeFromString(config.config.alarmTime);
  uint16_t currentMinutes = localTime.hour() * 60 + localTime.minute();
  if (currentMinutes == alarmMinutes && config.config.alarmActive)
  {
    return true;
  }
  else
  {
    return false;
  };
}

RgbColor Rainbow(uint8_t WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return RgbColor(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return RgbColor(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return RgbColor(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

RgbColor Alarm(uint8_t WheelPos)
{
  if (WheelPos % 4 == 0)
  {
    return RgbColor(htmlToColor("#ff0000"));
  }
  else
  {
    return RgbColor(0, 0, 0);
  }
}

uint64_t lasttime = 0;

bool tick()
{
  uint64_t diff = micros64() - lasttime;

  if (diff >= 16000)
  {
    lasttime = micros();
    return true;
  }
  else if (diff < 0)
  {
    lasttime = micros();
    return true;
  }
  return false;
}

void updateColors()
{

  if (isNight())
  {
    hourColorString = config.config.hourColorDimmed;
    minuteColorString = config.config.minuteColorDimmed;
    secondColorString = config.config.secondColorDimmed;
    hourDotString = config.config.hourDotColorDimmed;
    hourQuarterString = config.config.hourQuarterColorDimmed;
    hourSegmentString = config.config.hourSegmentColorDimmed;
  }
  else
  {
    hourColorString = config.config.hourColor;
    minuteColorString = config.config.minuteColor;
    secondColorString = config.config.secondColor;
    hourDotString = config.config.hourDotColor;
    hourQuarterString = config.config.hourQuarterColor;
    hourSegmentString = config.config.hourSegmentColor;
  }
  hourColor = htmlToColor(hourColorString);
  minuteColor = htmlToColor(minuteColorString);
  secondColor = htmlToColor(secondColorString);
  dot = htmlToColor(hourDotString);
  quarter = htmlToColor(hourQuarterString);
  segment = htmlToColor(hourSegmentString);
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

void alarmAnimation()
{

  for (uint16_t i = 0; i < config.config.ledCount; i++)
  {
    animationPos++;
    animationColor = Alarm(animationPos);
    uint8_t pixelPos = (i + second()) % 60;
    strip->SetPixelColor(pixelPos, animationColor);
  }
  strip->Show();
}

void showClockHandles()
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
  setPixel(calculateSecondHand(), secondColor, config.config.blendColors);

  strip->Show();
}

void hourRainbow()
{
  for (uint16_t j = 0; j < 256 * 5; j++)
  {
    for (uint16_t i = 0; i < config.config.ledCount; i++)
    {
      animationPos = ((i * 256 / config.config.ledCount) + j) & 0xFF;
      animationColor = Rainbow(animationPos);
      strip->SetPixelColor(i, animationColor);
    }
    animationColor.Darken(100);
  }
  strip->Show();
}

void loop()
{
#if defined(ESP8266)
  MDNS.update();
#endif
  webserver.handleRequest();
  uint8_t sec = second();
  uint8_t min = minute();

  if (isAlarm() && tick())
  {
    alarmAnimation();
  }
  else if (currentMinute == 0 && tick() && !isAlarm())
  {
    hourRainbow();
  }
  else if (currentSecond != sec && !isAlarm())
  {
    currentSecond = second();
    showClockHandles();
    animationPos = 0;
  }
  else if (currentMinute != min)
  {
    currentMinute = minute();
    updateColors();
  }
}
