#include "Animation.h"

#include "AnimationTrack.h"
#include "Scene/Node.h"
#include <memory>

namespace re {

Animation::Animation()
    :looping(false), power(1), currentStackIndex(0), isUseAnimStack(false), currTime(0), beginTime(0)
{
}

float Animation::getAnimationPower() const
{
    return this->power;
}

void Animation::setAnimationPower(float power)
{
    this->power = power;
}

bool Animation::isAnimationLoop() const
{
    return this->looping;
}

void Animation::setAnimationLoop(bool loop)
{
    this->looping = loop;
}

void Animation::setAnimationStackIndex(Int index)
{
    this->currentStackIndex = index;
}

void Animation::setIsUseAnimationStack(bool use)
{
    this->isUseAnimStack = use;
}

void Animation::addAnimationTrack(AnimationTrackPtr track)
{
    track->animation = this->shared_from_this();
    this->animTracks.push_back(track);
}

void Animation::addAnimationStack(AnimationStackPtr stack)
{
    this->animStacks.push_back(stack);
}

AnimationStackPtr Animation::getCurrAnimationStack()
{
    if (this->isUseAnimStack
            &&(this->currentStackIndex >= 0)
            && (this->currentStackIndex < this->animStacks.size())) {
        return this->animStacks.at(this->currentStackIndex);
    }

    return nullptr;
}

AnimationTrackPtr Animation::getCurrAnimationTrack()
{
    return this->animTracks.at(0);
}
Long Animation::getCurrTime() const
{
    return currTime;
}

void Animation::setCurrTime(const Long &value)
{
    currTime = value;
}
Long Animation::getBeginTime() const
{
    return beginTime;
}

void Animation::setBeginTime(const Long &value)
{
    beginTime = value;
}

AnimationStack::AnimationStack(Long begin, Long end)
{
    this->stackBeginTime = begin;
    this->stackEndTime = end;

    this->stackLength = end - begin;
}

Long AnimationStack::getStackBeginTime() const
{
    return stackBeginTime;
}

Long AnimationStack::getStackEndTime() const
{
    return stackEndTime;
}

}
