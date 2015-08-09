#include "VelocityTracker.h"

namespace re
{

void VelocityTracker::addPoint(const Vec2 &p, int64_t time)
{
    int newBegin = _begine;
    for (int i = 0; i <= _index; ++i) {
        int index = poolIndexToArrayIndex(_begine, i);
        if (_pastTime[index] < time - LONGEST_PAST_TIME)
        {
            newBegin = _begine + i + 1;
        }
    }

    int offset = newBegin - _begine;
    _begine = newBegin % NUM_PAST;
    _index -= offset;

    _index ++;
    int index = poolIndexToArrayIndex(_begine, _index);
    this->_pastPoint[index] = p;
    this->_pastTime[index] = time;
}

void VelocityTracker::computeVelocity()
{
    int units = 1000;

    int oldestIndex = poolIndexToArrayIndex(_begine, 0);
    Vec2 oldestPoint = _pastPoint[oldestIndex];
    int64_t oldestTime = _pastTime[oldestIndex];

    Vec2 result;
    for (int i = 0; i <= _index; ++i) {
        int index = poolIndexToArrayIndex(_begine, i);

        int64_t dur = _pastTime[index] - oldestTime;
        Vec2 dist = _pastPoint[index] - oldestPoint;
        Vec2 velocity = dist / dur / units;

        if (i == 0) {
            result = velocity;
        } else {
            result = (result + velocity) * 0.5f;
        }
    }

    _velocity = result;
}

const Vec2 &VelocityTracker::getVelocity()
{
    return this->_velocity;
}

int VelocityTracker::poolIndexToArrayIndex(int begin, int index)
{
    return (begin + index) % NUM_PAST;
}

}
