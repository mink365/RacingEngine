#include "ScrollAnimation.h"

#include "GameHub.h"
#include "Math/Math.h"

namespace re {
namespace ui {

void BaseAnimation::update()
{
    if (_state != State::Running) {
        return;
    }

    _runTime += GameHub::instance().GetDeltaTime().GetSecond();

    if (_runTime >= _duration) {
        this->switchState(State::Stopped);
    }
}

void BaseAnimation::start()
{
    this->switchState(State::Running);
}

void BaseAnimation::stop()
{
    this->switchState(State::Stopped);
}

void BaseAnimation::switchState(BaseAnimation::State state)
{
    if (_state == state) {
        return;
    }

    _state = state;
}


void BackAnimation::start(float currPos, float endPos, float time)
{
    this->_duration = time;
    this->_distance = endPos - currPos;

    this->_endPos = endPos;

    BaseAnimation::start();
}

float BackAnimation::getPosition()
{
    if (_state == State::Stopped) {
        return _endPos;
    }

    float timeRemain = _duration - _runTime;
    float timePercent = timeRemain / _duration;

    float offset = timePercent * timePercent * _distance;

    return _endPos - offset;
}

float BackAnimation::getDistance()
{
    return _endPos - getPosition();
}

void FlipAnimation::start(float currPos, float velocity)
{
    _dcc = 5;

    _realDcc = _dcc * MathLib::sign(velocity);

    float dccTime = velocity / _dcc;
    float maxDistance = velocity * dccTime / 2;

    _endPos = currPos + maxDistance;
    _duration = dccTime;

    BaseAnimation::start();
}

float FlipAnimation::getPosition()
{
    if (_state == State::Stopped) {
        return _endPos;
    }

    // a * T^2 / 2f + start = end --> start = end - a * T^2 / 2f
    // curr = start + v * t - a * t^2 / 2f --> curr = end - (a * T^2 / 2f - a * T * t + a * t^2 / 2f)
    float dis = _realDcc * (_duration * _duration / 2.0f - _duration * _runTime + _runTime * _runTime  / 2.0f);
    return _endPos - dis;
}

float FlipAnimation::calculateStartVelocity(float currPos, float endPos)
{
    float offset = endPos - currPos;
    return MathLib::sqrt(2 * MathLib::abs(offset) * _dcc) * MathLib::sign(offset);
}

} // namespace ui
} // namespace re

