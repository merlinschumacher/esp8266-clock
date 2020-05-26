#include <ezTime.h>
#include <NeoPixelBus.h>
#include "webserver.h"
#include "config.h"

Config config;
Timezone localTime;
Webserver webserver;

WiFiClient espClient;
PubSubClient client(espClient);

uint8_t currentMinute = 60;
uint8_t currentSecond = 60;

uint8_t animationPos = 0;

String hourColorString, minuteColorString, secondColorString, hourDotString, hourQuarterString, hourSegmentString;
RgbColor animationColor;
RgbColor off(0, 0, 0);
RgbColor hourColor(0, 0, 0);
RgbColor minuteColor(0, 0, 0);
RgbColor secondColor(0, 0, 0);
RgbColor dot(0, 0, 0);
RgbColor quarter(0, 0, 0);
RgbColor segment(0, 0, 0);

#if defined(ESP8266)
NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBangWs2812xMethod> *strip = NULL;
#elif defined(ESP32)
NeoPixelBus<NeoGrbFeature, NeoEsp32BitBangWs2812xMethod> *strip = NULL;
#endif