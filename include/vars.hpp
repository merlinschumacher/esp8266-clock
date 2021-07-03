#ifndef vars_h
#define vars_h

Config config;
Timezone localTime;
Webserver webserver;

WiFiClient espClient;

uint8_t currentMinute = 60;
uint8_t currentSecond = 60;
uint8_t currentHour = 24;
uint8_t currentDayPos = 32;
uint8_t currentWeekdayPos = 8;
uint8_t currentMonthPos = 13;
bool night = true;

uint32_t frame = 0;

NeoPixelAnimator animations(3); // NeoPixel animation management object

String hourColorString, minuteColorString, secondColorString, hourDotString, hourQuarterString, hourSegmentString, bgColorString, dayColorString, monthColorString, weekdayColorString;
RgbColor off(0, 0, 0);
RgbColor hourColor(0, 0, 0);
RgbColor minuteColor(0, 0, 0);
RgbColor secondColor(0, 0, 0);
RgbColor dot(0, 0, 0);
RgbColor quarter(0, 0, 0);
RgbColor segment(0, 0, 0);
RgbColor bgColor(0, 0, 0);
RgbColor dayColor(0, 0, 0);
RgbColor weekdayColor(0, 0, 0);
RgbColor monthColor(0, 0, 0);

#if defined(ESP8266)
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod> *strip = NULL;
NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBangWs2812xMethod> *bgStrip = NULL;
#elif defined(ESP32)
NeoPixelBus<NeoGrbFeature, NeoEsp32BitBangWs2812xMethod> *strip = NULL;
NeoPixelBus<NeoGrbFeature, NeoEsp32BitBangWs2812xMethod> *bgStrip = NULL;
#endif

#endif //vars_h