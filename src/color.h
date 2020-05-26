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
        strip->Show();
        animationColor.Darken(100);
    }
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

RgbColor DimColor(float percent, RgbColor sourceColor)
{
    RgbColor targetColor(0, 0, 0);
    percent = 100 - percent;
    targetColor.R = (sourceColor.R * percent) / 100;
    targetColor.G = (sourceColor.G * percent) / 100;
    targetColor.B = (sourceColor.B * percent) / 100;
    return targetColor;
}

#endif //color_h