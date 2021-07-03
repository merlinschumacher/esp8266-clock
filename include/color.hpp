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
    int length = 0;

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
    }
}

void renderRainbow(bool night = false, bool bg = false)
{
    int length = 0;
    HslColor animationColor(0, 0, 0);

    float lightness = 1.0;
    if (night)
    {
        lightness = 0.1;
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
        animationColor = HslColor(hue, 1.00, lightness);

        if (bg)
        {
            bgStrip->SetPixelColor(i, animationColor);
        }
        else
        {
            strip->SetPixelColor(i, animationColor);
        }
    }
}

void updateColors(bool isNight = false)
{

    if (isNight)
    {
        hourColorString = config.config.hourColorDimmed;
        minuteColorString = config.config.minuteColorDimmed;
        secondColorString = config.config.secondColorDimmed;
        hourDotString = config.config.hourDotColorDimmed;
        hourQuarterString = config.config.hourQuarterColorDimmed;
        hourSegmentString = config.config.hourSegmentColorDimmed;
        bgColorString = config.config.bgColorDimmed;
        monthColorString = config.config.monthColorDimmed;
        dayColorString = config.config.dayColorDimmed;
        weekdayColorString = config.config.weekdayColorDimmed;
    }
    else
    {
        hourColorString = config.config.hourColor;
        minuteColorString = config.config.minuteColor;
        secondColorString = config.config.secondColor;
        hourDotString = config.config.hourDotColor;
        hourQuarterString = config.config.hourQuarterColor;
        hourSegmentString = config.config.hourSegmentColor;
        bgColorString = config.config.bgColor;
        monthColorString = config.config.monthColor;
        dayColorString = config.config.dayColor;
        weekdayColorString = config.config.weekdayColor;
    }
    hourColor = htmlToColor(hourColorString);
    minuteColor = htmlToColor(minuteColorString);
    secondColor = htmlToColor(secondColorString);
    dot = htmlToColor(hourDotString);
    quarter = htmlToColor(hourQuarterString);
    segment = htmlToColor(hourSegmentString);
    bgColor = htmlToColor(bgColorString);
    dayColor = htmlToColor(dayColorString);
    monthColor = htmlToColor(monthColorString);
    weekdayColor = htmlToColor(weekdayColorString);
}

#endif //color_h