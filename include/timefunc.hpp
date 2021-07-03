#ifndef timefunc_h
#define timefunc_h
#include <ezTime.h>
uint64_t lasttime = 0;

bool tick()
{
    unsigned long diff = micros() - lasttime;

    if (diff >= 16666 || diff < 0)
    {
        lasttime = micros();
        frame++;
        return true;
    }
    return false;
}

bool isNight(uint8_t h, uint8_t m)
{
    uint16_t currentMinutes = h * 60 + m;
    if (config.config.nightTimeBegins > config.config.nightTimeEnds)
    {
        if (config.config.nightTimeBegins <= currentMinutes || currentMinutes <= config.config.nightTimeEnds)
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
        if (config.config.nightTimeBegins <= currentMinutes && currentMinutes <= config.config.nightTimeEnds)
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
    uint16_t currentMinutes = currentHour * 60 + currentMinute;
    if (currentMinutes == config.config.alarmTime && config.config.alarmActive)
    {
        return true;
    }
    else
    {
        return false;
    };
}

#endif //timefunc_h