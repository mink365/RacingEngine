#include "AxisData.h"
#include "GameHub.h"
#include "ScrollView.h"

namespace re {
namespace ui {

AxisData::AxisData(const ScrollParam& param)
    : param(param)
{
    flipAnim._dcc = param.flickDecc;
    std::function<void(BaseAnimation::State)> func = [&](BaseAnimation::State state)
    {
        if (state == BaseAnimation::State::Stopped) {
            this->fixup();
        }
    };
    flipAnim.stateEvent += func;

    func = [&](BaseAnimation::State state)
    {
        if (state == BaseAnimation::State::Stopped) {
            this->switchState(MoveState::Stopped);
        }
    };
    backAnim.stateEvent += func;
}

void AxisData::update()
{
    switch(this->state)
    {
    case MoveState::Backing:
        backAnim.update();

        currentPos = backAnim.getPosition();
        break;
    case MoveState::Flicking:
        flipAnim.update();

        currentPos = flipAnim.getPosition();
        break;
    default:
        break;
    }
}

void AxisData::reset()
{
    this->switchState(MoveState::Stopped);

    flipAnim.stop();
    backAnim.stop();

    currentVelocity = 0;
}

void AxisData::drag(float diffValue)
{
    this->switchState(MoveState::Dragging);

    float newPos = pressPos + diffValue;

    if (newPos > maxBound) {
        if (param.boundBehavior == BoundBehavior::StopAtBound
                || param.boundBehavior == BoundBehavior::StopAtGrid) {
            newPos = maxBound;
        } else {
            newPos = maxBound + (newPos - maxBound) / 2.0f;
        }
    } else if (newPos < minBound) {
        if (param.boundBehavior == BoundBehavior::StopAtBound
                || param.boundBehavior == BoundBehavior::StopAtGrid) {
            newPos = minBound;
        } else {
            newPos = maxBound + (newPos - maxBound) / 2.0f;
        }
    }

    this->currentPos = newPos;
}

void AxisData::flick(float velocity)
{
    this->switchState(MoveState::Flicking);

    float target = velocity > 0 ? maxBound : minBound;

    if (param.boundBehavior == BoundBehavior::StopAtBound
            || param.boundBehavior == BoundBehavior::StopAtGrid) {

        float newVelocity = flipAnim.calculateStartVelocity(currentPos, target);

        if (MathLib::abs(newVelocity) > MathLib::abs(velocity)) {
            velocity = newVelocity;
        }
    }

    flipAnim.start(currentPos, velocity);
}

void AxisData::fixup()
{
    if (param.boundBehavior == BoundBehavior::StopAtGrid) {

    } else {
        if (this->currentPos > maxBound) {
            this->switchState(MoveState::Backing);

            backAnim.start(currentPos, maxBound, 0.5f);
        } else if (this->currentPos < minBound) {
            this->switchState(MoveState::Backing);

            backAnim.start(currentPos, minBound, 0.5f);
        } else {
            this->switchState(MoveState::Stopped);
        }
    }
}

void AxisData::switchState(MoveState state)
{
    if (this->state == state) {
        return;
    }

    this->state = state;
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

void BaseAnimation::update()
{
    if (_state != State::Running) {
        return;
    }

    _runTime += GameHub::getInstance().GetDeltaTime().GetSecond();

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

} // namespace ui
} // namespace re

