/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "Time.h"

Time::Time()
    : _ns(0)
{
}

Time::Time(uint64_t ns)
    : _ns(ns)
{

}

void Time::SetNanoTime(uint64_t ns)
{
    this->_ns = ns;
}

void Time::SetTime(uint64_t ms)
{
    this->_ns = ms * TimeConstant::NANOSECONDS_PER_MILLISECOND;
}

uint64_t Time::GetNanoSecond() const
{
    return _ns;
}

uint64_t Time::GetMilliSecond() const
{
    return _ns * TimeConstant::MILLISECONDS_PER_NANOSECOND;
}

float Time::GetSecond() const
{
    return _ns * TimeConstant::SECONDS_PER_NANOSECOND;
}

float Time::GetMinute() const
{
    return GetSecond() / TimeConstant::SECONDS_PER_MINUTE;
}

float Time::GetHour() const
{
    return GetSecond() / TimeConstant::SECONDS_PER_HOUR;
}

