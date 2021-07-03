#ifndef led_h
#define led_h
#include <NeoPixelBus.h>

void initStrip()
{
    if (strip != NULL)
    {
        delete strip;
    }
#if defined(ESP8266)
    strip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod>(config.config.ledCount, config.config.ledPin);
    bgStrip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBangWs2812xMethod>(config.config.bgLedCount, config.config.bgLedPin);
#elif defined(ESP32)
    strip = new NeoPixelBus<NeoGrbFeature, NeoEsp32BitBangWs2812xMethod>(config.config.ledCount, config.config.ledPin);
    bgStrip = new NeoPixelBus<NeoGrbFeature, NeoEsp32BitBangWs2812xMethod>(config.config.bgLedCount, config.config.bgLedPin);
#endif
    strip->Begin();
    strip->ClearTo(off);
    strip->Show();
    bgStrip->Begin();
    bgStrip->ClearTo(off);
    bgStrip->Show();
}

void setPixel(uint8_t pos, RgbColor color, bool blend = true)
{
    if (blend)
    {
        RgbColor currentcolor = strip->GetPixelColor(pos);
        if (currentcolor.CalculateBrightness() == 0)
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
#endif //led_h