#ifndef vars_h
#define vars_h

Config config;
Timezone localTime;
Webserver webserver;
WiFiClient espClient;
Mqtt mqtt;

uint8_t currentMinute = 60,
        currentSecond = 60,
        currentHour = 24,
        currentDayPos = 32,
        currentWeekdayPos = 8,
        currentMonthPos = 13;

bool night = true,
     alarm = false,
     topHour = false,
     animationRendered = false;

uint32_t frame = 0;

RgbColor off(0, 0, 0),
    hourColor(0, 0, 0),
    minuteColor(0, 0, 0),
    secondColor(0, 0, 0),
    dot(0, 0, 0),
    quarter(0, 0, 0),
    segment(0, 0, 0),
    bgColor(0, 0, 0),
    dayColor(0, 0, 0),
    weekdayColor(0, 0, 0),
    monthColor(0, 0, 0);

#if defined(ESP8266)
#if defined(UART_MODE)
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod> *strip = NULL;
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart0800KbpsMethod> *bgStrip = NULL;
#elif defined(BITBANG_MODE)
NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBangWs2812xMethod> *strip = NULL;
NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBangWs2812xMethod> *bgStrip = NULL;
#elif defined(DMA_MODE)
NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> *strip = NULL;
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod> *bgStrip = NULL;
#elif defined(DEBUG_BUILD)
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod> *strip = NULL;
NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBangWs2812xMethod> *bgStrip = NULL;
#endif
#elif defined(ESP32)
NeoPixelBus<NeoGrbFeature, NeoEsp32BitBangWs2812xMethod> *strip = NULL;
NeoPixelBus<NeoGrbFeature, NeoEsp32BitBangWs2812xMethod> *bgStrip = NULL;
#endif

#endif //vars_h