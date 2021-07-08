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
#if defined(UART_MODE)
    strip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod>(config.config.ledCount, config.config.ledPin);
    bgStrip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart0800KbpsMethod>(config.config.bgLedCount, config.config.bgLedPin);
#elif defined(BITBANG_MODE)
    strip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBangWs2812xMethod>(config.config.ledCount, config.config.ledPin);
    bgStrip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBangWs2812xMethod>(config.config.bgLedCount, config.config.bgLedPin);
#elif defined(DMA_MODE)
    strip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod>(config.config.ledCount, config.config.ledPin);
    bgStrip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod>(config.config.bgLedCount, config.config.bgLedPin);
#elif defined(DEBUG_BUILD)
    strip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod>(config.config.ledCount, config.config.ledPin);
    bgStrip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBangWs2812xMethod>(config.config.bgLedCount, config.config.bgLedPin);
#endif
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

void setPixel(uint8_t pos, HsbColor color, bool blend = true)
{
    if (color.B == 0)
        return;
    if (blend)
    {
        HsbColor currentColor = strip->GetPixelColor(pos);
        if (currentColor.B == 0)
        {
            strip->SetPixelColor(pos, color);
        }
        else
        {
            HsbColor targetColor;
            float targetBlend = color.B;
            float targetBrightness = max(currentColor.B, color.B);
            float targetSaturation = max(currentColor.S, color.S);
            if (targetSaturation == 0)
            {
                currentColor.H = color.H;
                targetSaturation = color.S;
            }
            targetColor = RgbColor::LinearBlend(currentColor, color, targetBlend);
            targetColor.B = targetBrightness;
            strip->SetPixelColor(pos, targetColor);
        }
    }
    else
    {
        strip->SetPixelColor(pos, color);
    }
}
#endif //led_h