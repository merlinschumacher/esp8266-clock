#ifndef color_h
#define color_h
#include <NeoPixelBus.h>

HtmlColor htmlToColor(String color)
{
    HtmlColor htmlconv;
    htmlconv.Parse<HtmlShortColorNames>(color);
    RgbColor rgb(htmlconv);
    return rgb;
}

void renderAlarm(bool night = false, bool bg = false)
{
    int length = 1;

    float brightness = 1.0;
    if (night)
    {
        brightness = 0.1;
    }
    HsbColor animationColor(1, 1, brightness);

    if (bg)
    {
        bgStrip->ClearTo(off);
        length = config.config.bgLedCount;
    }
    else
    {

        strip->ClearTo(off);
        length = config.config.ledCount;
    }

    for (uint16_t i = 0; i < length; i++)
    {
        if (i % 4 == 0)
        {
            if (bg)
            {
                bgStrip->SetPixelColor(i, animationColor);
            }
            else
            {
                strip->SetPixelColor(i, animationColor);
            }
        }
        yield();
    }
}

void renderRainbow(bool night = false, bool bg = false)
{
    int length = 1;

    float brightness = 1.0;
    if (night)
    {
        brightness = 0.1;
    }

    if (bg)
    {
        bgStrip->ClearTo(off);
        length = config.config.bgLedCount;
    }
    else
    {

        strip->ClearTo(off);
        length = config.config.ledCount;
    }

    float hueStep = 1.0 / (float)length;
    for (uint16_t i = 0; i < length; i++)
    {
        float hue = hueStep * i;
        HsbColor animationColor(hue, 1, brightness);

        if (bg)
        {
            bgStrip->SetPixelColor(i, animationColor);
        }
        else
        {
            strip->SetPixelColor(i, animationColor);
        }
        yield();
    }
}

void updateColors(bool isNight = false)
{

    if (isNight)
    {
        hourColor = htmlToColor(config.config.hourColorDimmed);
        minuteColor = htmlToColor(config.config.minuteColorDimmed);
        secondColor = htmlToColor(config.config.secondColorDimmed);
        dot = htmlToColor(config.config.hourDotColorDimmed);
        quarter = htmlToColor(config.config.hourQuarterColorDimmed);
        segment = htmlToColor(config.config.hourSegmentColorDimmed);
        bgColor = htmlToColor(config.config.bgColorDimmed);
        dayColor = htmlToColor(config.config.dayColorDimmed);
        monthColor = htmlToColor(config.config.monthColorDimmed);
        weekdayColor = htmlToColor(config.config.weekdayColorDimmed);
    }
    else
    {
        hourColor = htmlToColor(config.config.hourColor);
        minuteColor = htmlToColor(config.config.minuteColor);
        secondColor = htmlToColor(config.config.secondColor);
        dot = htmlToColor(config.config.hourDotColor);
        quarter = htmlToColor(config.config.hourQuarterColor);
        segment = htmlToColor(config.config.hourSegmentColor);
        bgColor = htmlToColor(config.config.bgColor);
        dayColor = htmlToColor(config.config.dayColor);
        monthColor = htmlToColor(config.config.monthColor);
        weekdayColor = htmlToColor(config.config.weekdayColor);
    }
}

#endif //color_h