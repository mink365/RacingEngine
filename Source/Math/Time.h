/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef TIME_H
#define TIME_H

#include <stdint.h>

namespace TimeConstant
{
    const uint64_t MONTHS_PER_YEAR = 12;

    const uint64_t DAYS_PER_WEEK = 7;

    const uint64_t DAYS_PER_MONTH = 30;

    const uint64_t HOURS_PER_DAY = 24;

    const uint64_t MINUTES_PER_HOUR = 60;

    const uint64_t MILLISECONDS_PER_SECOND = 1000;
    const uint64_t MICROSECONDS_PER_SECOND = 1000 * 1000;
    const uint64_t NANOSECONDS_PER_SECOND = 1000 * 1000 * 1000;

    const uint64_t MICROSECONDS_PER_MILLISECOND = MICROSECONDS_PER_SECOND / MILLISECONDS_PER_SECOND;

    const uint64_t NANOSECONDS_PER_MICROSECOND = NANOSECONDS_PER_SECOND / MICROSECONDS_PER_SECOND;
    const uint64_t NANOSECONDS_PER_MILLISECOND = NANOSECONDS_PER_SECOND / MILLISECONDS_PER_SECOND;

    const double SECONDS_PER_NANOSECOND = 1.0 / NANOSECONDS_PER_SECOND;
    const double MICROSECONDS_PER_NANOSECOND = 1.0 / NANOSECONDS_PER_MICROSECOND;
    const double MILLISECONDS_PER_NANOSECOND = 1.0 / NANOSECONDS_PER_MILLISECOND;

    const double SECONDS_PER_MICROSECOND = 1.0 / MICROSECONDS_PER_SECOND;
    const double MILLISECONDS_PER_MICROSECOND = 1.0 / MICROSECONDS_PER_MILLISECOND;

    const double SECONDS_PER_MILLISECOND = 1.0 / MILLISECONDS_PER_SECOND;

    const uint64_t SECONDS_PER_MINUTE = 60;
    const uint64_t SECONDS_PER_HOUR = SECONDS_PER_MINUTE * MINUTES_PER_HOUR;
    const uint64_t SECONDS_PER_DAY = SECONDS_PER_HOUR * HOURS_PER_DAY;
    const uint64_t SECONDS_PER_WEEK = SECONDS_PER_DAY * DAYS_PER_WEEK;
    const uint64_t SECONDS_PER_MONTH = SECONDS_PER_DAY * DAYS_PER_MONTH;
    const uint64_t SECONDS_PER_YEAR = SECONDS_PER_MONTH * MONTHS_PER_YEAR;
}

class Time
{
public:
    Time();
    Time(uint64_t ns);

    uint64_t GetNanoSecond() const;
    uint64_t GetMilliSecond() const;

    float GetSecond() const;
    float GetMinute() const;
    float GetHour() const;

    void SetNanoTime(uint64_t ns);
    void SetTime(uint64_t ms);

private:
    uint64_t _ns;
};

#endif // TIME_H
