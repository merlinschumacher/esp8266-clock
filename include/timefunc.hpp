#ifndef timefunc_h
#define timefunc_h
#include <ezTime.h>
uint64_t lasttime = 0;

bool tick()
{
    int64_t diff = ms() - lasttime;

    if (diff >= 16 || diff < 0)
    {
        lasttime = ms();
        return true;
    }
    return false;
}

uint16_t timeFromString(char *str)
{
    int hour, minute;
    sscanf(str, "%i:%i", &hour, &minute);
    return hour * 60 + minute;
}

bool isNight()
{
    uint16_t currentMinutes = localTime.hour() * 60 + localTime.minute();
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
    uint16_t currentMinutes = localTime.hour() * 60 + localTime.minute();
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