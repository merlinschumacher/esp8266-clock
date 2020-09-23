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

RgbColor DimColor(float percent, RgbColor sourceColor)
{
    RgbColor targetColor(0, 0, 0);
    percent = 100 - percent;
    targetColor.R = (sourceColor.R * percent) / 100;
    targetColor.G = (sourceColor.G * percent) / 100;
    targetColor.B = (sourceColor.B * percent) / 100;
    return targetColor;
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

void alarmAnimation(bool isNight = false)
{

    for (uint16_t i = 0; i < config.config.ledCount; i++)
    {
        animationPos++;
        animationColor = Alarm(animationPos);

        if (isNight)
        {
            animationColor = DimColor(90, animationColor);
        }
        uint8_t pixelPos = (i + second()) % 60;
        strip->SetPixelColor(pixelPos, animationColor);
        if (config.config.bgLight)
        {
            bgStrip->SetPixelColor(pixelPos, animationColor);
        }
    }
    strip->Show();
    if (config.config.bgLight)
    {
        bgStrip->Show();
    }
}

void hourRainbow(bool isNight = false)
{
    for (uint16_t j = 0; j < 256; j++)
    {
        for (uint16_t i = 0; i < config.config.ledCount; i++)
        {
            animationPos = ((i * 256 / config.config.ledCount) + j) & 0xFF;
            RgbColor rainbow = Rainbow(animationPos);
            animationColor = rainbow;
            if (isNight)
            {
                rainbow = DimColor(90, rainbow);
            }
            strip->SetPixelColor(i, rainbow);
            if (config.config.bgLight)
            {
                bgStrip->SetPixelColor(i, rainbow);
            }
        }
        strip->Show();
        if (config.config.bgLight)
        {
            bgStrip->Show();
        }
        animationColor.Darken(100);
        yield();
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