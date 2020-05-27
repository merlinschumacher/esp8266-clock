#ifndef timefunc_h
#define timefunc_h
#include <ezTime.h>
uint64_t lasttime = 0;

bool tick()
{
    uint64_t diff = ms() - lasttime;

    if (diff >= 16)
    {
        lasttime = ms();
        return true;
    }
    else if (diff < 0)
    {
        lasttime = ms();
        return true;
    }
    return false;
}

String getStringPartByNr(String data, char separator, int index)
{

    uint8_t stringData = 0;
    String dataPart = "";
    for (uint8_t i = 0; i < data.length(); i++)
    {
        if (data[i] == separator)
        {
            stringData++;
        }
        else if (stringData == index)
        {
            dataPart.concat(data[i]);
        }
        else if (stringData > index)
        {
            return dataPart;
            break;
        }
    }
    return dataPart;
}

uint16_t timeFromString(String time)
{
    uint8_t hour, minute;
    String hourString = getStringPartByNr(time, ':', 0);
    String minuteString = getStringPartByNr(time, ':', 1);
    hour = hourString.toInt();
    minute = minuteString.toInt();
    return hour * 60 + minute;
}

bool isNight()
{
    uint16_t nightStartMinutes = timeFromString(config.config.nightTimeBegins);
    uint16_t nightEndMinutes = timeFromString(config.config.nightTimeEnds);
    uint16_t currentMinutes = localTime.hour() * 60 + localTime.minute();
    if (nightStartMinutes > nightEndMinutes)
    {
        if (nightStartMinutes <= currentMinutes || currentMinutes <= nightEndMinutes)
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
        if (nightStartMinutes <= currentMinutes && currentMinutes <= nightEndMinutes)
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
    uint16_t alarmMinutes = timeFromString(config.config.alarmTime);
    uint16_t currentMinutes = localTime.hour() * 60 + localTime.minute();
    if (currentMinutes == alarmMinutes && config.config.alarmActive)
    {
        return true;
    }
    else
    {
        return false;
    };
}

#endif //timefunc_h