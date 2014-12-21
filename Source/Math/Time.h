/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef TIME_H
#define TIME_H

#include <stdint.h>

#pragma once

namespace TimeConstant
{
    const int MONTHS_PER_YEAR = 12;

    const int DAYS_PER_WEEK = 7;

    const int DAYS_PER_MONTH = 30;

    const int HOURS_PER_DAY = 24;

    const int MINUTES_PER_HOUR = 60;

    const int MILLISECONDS_PER_SECOND = 1000;
    const int MICROSECONDS_PER_SECOND = 1000 * 1000;
    const long NANOSECONDS_PER_SECOND = 1000 * 1000 * 1000;

    const long MICROSECONDS_PER_MILLISECOND = MICROSECONDS_PER_SECOND / MILLISECONDS_PER_SECOND;

    const long NANOSECONDS_PER_MICROSECOND = NANOSECONDS_PER_SECOND / MICROSECONDS_PER_SECOND;
    const long NANOSECONDS_PER_MILLISECOND = NANOSECONDS_PER_SECOND / MILLISECONDS_PER_SECOND;

    const float SECONDS_PER_NANOSECOND = 1.0f / NANOSECONDS_PER_SECOND;
    const float MICROSECONDS_PER_NANOSECOND = 1.0f / NANOSECONDS_PER_MICROSECOND;
    const float MILLISECONDS_PER_NANOSECOND = 1.0f / NANOSECONDS_PER_MILLISECOND;

    const float SECONDS_PER_MICROSECOND = 1.0f / MICROSECONDS_PER_SECOND;
    const float MILLISECONDS_PER_MICROSECOND = 1.0f / MICROSECONDS_PER_MILLISECOND;

    const float SECONDS_PER_MILLISECOND = 1.0f / MILLISECONDS_PER_SECOND;

    const int SECONDS_PER_MINUTE = 60;
    const int SECONDS_PER_HOUR = SECONDS_PER_MINUTE * MINUTES_PER_HOUR;
    const int SECONDS_PER_DAY = SECONDS_PER_HOUR * HOURS_PER_DAY;
    const int SECONDS_PER_WEEK = SECONDS_PER_DAY * DAYS_PER_WEEK;
    const int SECONDS_PER_MONTH = SECONDS_PER_DAY * DAYS_PER_MONTH;
    const int SECONDS_PER_YEAR = SECONDS_PER_MONTH * MONTHS_PER_YEAR;
}

class Time
{
public:
    Time(uint64_t ns);

    uint64_t GetNanoSecond() const;
    uint64_t GetMillSecond() const;

    float GetSecond() const;
    float GetMinute() const;
    float GetHour() const;

private:
    uint64_t _ns;
};

inline uint64_t Time::GetNanoSecond() const
{
    return _ns;
}

inline uint64_t Time::GetMillSecond() const
{
    return _ns / TimeConstant::NANOSECONDS_PER_SECOND;
}

inline float Time::GetSecond() const
{
    return _ns / (float)TimeConstant::NANOSECONDS_PER_SECOND;
}

inline float Time::GetMinute() const
{
    return GetSecond() / TimeConstant::SECONDS_PER_MINUTE;
}

inline float Time::GetHour() const
{
    return GetSecond() / TimeConstant::SECONDS_PER_HOUR;
}

#endif // TIME_H
