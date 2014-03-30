#include "Animation.h"

#include "BoneNode.h"
#include "scene/Node.h"

namespace re {

Animation::Animation()
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

Mat4 AnimationTrack::getLocalMatrix()
{
    // TODO: curr and begin time
//    this->currentTime = this->animation->

    Long timePos;

    this->calcProportion(timePos);

    return this->linearDeformation();
}

Int AnimationTrack::getCurrKeyFrameIndex()
{
    return this->currentFrameIndex;
}

Int AnimationTrack::getKeyFrameCount()
{
    return this->keyFrames.size();
}

void AnimationTrack::calcProportion(Long timePos)
{
    std::vector<KeyFramePtr>::iterator iter;
    for (iter = this->keyFrames.begin() + 1; iter != this->keyFrames.end(); ++iter) {
        KeyFramePtr keyFrame = *iter;
        KeyFramePtr lastKeyFrame = *(iter - 1);

        if (timePos >= lastKeyFrame->getTime()
                && timePos <= keyFrame->getTime()) {

            this->interpolationBeginKeyFrame = lastKeyFrame;
            this->interpolationEndKeyFrame = keyFrame;

            this->frameProportion = (timePos - this->interpolationBeginKeyFrame->getTime())
                    / (float)(this->interpolationEndKeyFrame->getTime()
                              - this->interpolationBeginKeyFrame->getTime());

            break;
        }
    }

    if (iter == this->keyFrames.end()){
        this->interpolationBeginKeyFrame = this->keyFrames.at(0);
        this->interpolationEndKeyFrame = this->keyFrames.at(0);

        this->frameProportion = 0;
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

Mat4 AnimationTrack::linearDeformation()
{
    Vec3 trans = slerp(this->interpolationBeginKeyFrame->getTranslation(),
                       this->interpolationEndKeyFrame->getTranslation(), this->frameProportion);

    Quat rotate = slerp(this->interpolationBeginKeyFrame->getRotation(),
                        this->interpolationEndKeyFrame->getRotation(), this->frameProportion);

    Vec3 scale = slerp(this->interpolationBeginKeyFrame->getScaling(),
                       this->interpolationEndKeyFrame->getScaling(), this->frameProportion);

    this->boneNode->setLocalTranslation(trans);
    this->boneNode->setLocalRotation(rotate);
    this->boneNode->setLocalScaling(scale);

    this->boneNode->updateLocalMatrix();

    return this->boneNode->getLocalMatrix();
}

void AnimationTrack::updateTrackInfo()
{
    this->minKeyTime = this->keyFrames.front()->getTime();
    this->maxKeyTime = this->keyFrames.back()->getTime();

    this->animLength = maxKeyTime - minKeyTime;
}

}
