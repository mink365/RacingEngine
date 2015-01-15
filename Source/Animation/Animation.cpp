#include "Animation.h"

#include "Scene/Node.h"
#include "Scene/Transform.h"
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
    stack->animation = this->shared_from_this();
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

KeyFrame::KeyFrame(const long time, const Vec3 &tran, const Quat &rotation, const Vec3 &scale)
{
    this->time = time;
    this->translation = tran;
    this->rotation = rotation;
    this->scaling = scale;
}

string KeyFrame::getName() const
{
    return this->name;
}

Long KeyFrame::getTime() const
{
    return this->time;
}

const Vec3 &KeyFrame::getTranslation() const
{
    return this->translation;
}

const Vec3 &KeyFrame::getScaling() const
{
    return this->scaling;
}

const Quat &KeyFrame::getRotation() const
{
    return this->rotation;
}

const Mat4 &KeyFrame::getMatrix() const
{
    return this->matrix;
}

void KeyFrame::setName(const string &value)
{
    this->name = value;
}

void KeyFrame::setTranslation(const Vec3 &value)
{
    this->translation = value;
}

void KeyFrame::setScaling(const Vec3 &value)
{
    this->scaling = value;
}

void KeyFrame::setRotation(const Quat &value)
{
    this->rotation = value;
}

void AnimationTrack::addKeyFrame(const KeyFrame &frame)
{
    auto framePtr = std::make_shared<KeyFrame>(frame);
    this->keyFrames.push_back(framePtr);
}

void AnimationTrack::updateLocalMatrix()
{
    AnimationPtr animation = this->animation.lock();
    AnimationStackPtr currStack = animation->getCurrAnimationStack();

    Long stackBeginTime = 0;
    Long stackEndTIme = 0;
    Long stackLongth = 0;
    if (currStack != nullptr) {
        stackBeginTime = currStack->stackBeginTime;
        stackEndTIme = currStack->stackEndTime;
        stackLongth = currStack->stackLength;
    } else {
        stackLongth = this->animLength;
    }

    Long timePos;

    float power = animation->getAnimationPower();
    if (animation->isAnimationLoop()) {
        Long time = (this->currentTime - this->beginTime) * power;

        timePos = time % stackLongth + stackBeginTime;
    } else {
        timePos = (this->currentTime - this->beginTime) * power + stackBeginTime;

        if (timePos > this->animLength + stackBeginTime) {
            timePos = this->animLength + stackBeginTime;
        }
    }

    this->calcProportion(timePos);

    this->linearDeformation();
}

Int AnimationTrack::getCurrKeyFrameIndex()
{
    return this->currentFrameIndex;
}

Int AnimationTrack::getKeyFrameCount()
{
    return this->keyFrames.size();
}

KeyFramePtr AnimationTrack::getKeyFrame(int index)
{
    return this->keyFrames[index];
}

void AnimationTrack::calcProportion(Long timePos)
{
    size_t i = 0;
    for (i = 1; i < this->keyFrames.size(); ++i) {
        KeyFramePtr lastKeyFrame = this->keyFrames[i - 1];
        KeyFramePtr keyFrame = this->keyFrames[i];

        if (timePos <= keyFrame->getTime()) {

            this->interpolationBeginKeyFrame = lastKeyFrame;
            this->interpolationEndKeyFrame = keyFrame;

            this->frameProportion = (timePos - this->interpolationBeginKeyFrame->getTime())
                    / (float)(this->interpolationEndKeyFrame->getTime()
                              - this->interpolationBeginKeyFrame->getTime());

            this->currentFrameIndex = i;

            break;
        }
    }

    if (i >= this->keyFrames.size()){
        this->interpolationBeginKeyFrame = this->keyFrames.at(0);
        this->interpolationEndKeyFrame = this->keyFrames.at(0);

        this->frameProportion = 0;

        this->currentFrameIndex = 0;
    }
}

Vec3 slerp(const Vec3& b, const Vec3& e, float proportion) {
    Vec3 diff = e - b;

    diff *= proportion;

    return b + diff;
}

Quat slerp(const Quat& b, const Quat& e, float proportion) {
    Quat temp = b;
    Quat tempEnd = e;

    temp.slerp(tempEnd, proportion);

    return temp;
}

void AnimationTrack::linearDeformation()
{
    Vec3 trans = slerp(this->interpolationBeginKeyFrame->getTranslation(),
                       this->interpolationEndKeyFrame->getTranslation(), this->frameProportion);

    Quat rotate = slerp(this->interpolationBeginKeyFrame->getRotation(),
                        this->interpolationEndKeyFrame->getRotation(), this->frameProportion);

    Vec3 scale = slerp(this->interpolationBeginKeyFrame->getScaling(),
                       this->interpolationEndKeyFrame->getScaling(), this->frameProportion);

    std::shared_ptr<Node> node =  this->node.lock();

    TransformPtr& transform = node->getTransform();
    transform->setLocalTranslation(trans);
    transform->setLocalRotation(rotate);
    transform->setLocalScaling(scale);
}

void AnimationTrack::updateTrackInfo()
{
    this->minKeyTime = this->keyFrames.front()->getTime();
    this->maxKeyTime = this->keyFrames.back()->getTime();

    this->animLength = maxKeyTime - minKeyTime;
}

void AnimationTrack::updateTimeInfo()
{
    AnimationPtr animation = this->animation.lock();

    this->currentTime = animation->getCurrTime();
    this->beginTime = animation->getBeginTime();
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
