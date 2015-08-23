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

} // namespace ui
} // namespace re

