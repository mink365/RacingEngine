﻿#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include <memory>
#include "platform.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

namespace re {

class Node;
class AnimationTrack;
class AnimationStack;
class Animation;
class KeyFrame;

typedef shared_ptr<Animation> AnimationPtr;
typedef shared_ptr<AnimationTrack> AnimationTrackPtr;
typedef shared_ptr<AnimationStack> AnimationStackPtr;
typedef shared_ptr<KeyFrame> KeyFramePtr;

class Animation : public enable_shared_from_this<Animation>
{
public:
    Animation();

    float getAnimationPower() const;
    void setAnimationPower(float power);

    bool isAnimationLoop() const;
    void setAnimationLoop(bool loop);

    void setAnimationStackIndex(Int index);

    void setIsUseAnimationStack(bool use);

    void addAnimationTrack(AnimationTrackPtr track);
    void addAnimationStack(AnimationStackPtr stack);

    AnimationStackPtr getCurrAnimationStack();
    AnimationTrackPtr getCurrAnimationTrack();

    Long getCurrTime() const;
    void setCurrTime(const Long &value);

    Long getBeginTime() const;
    void setBeginTime(const Long &value);

private:

    vector<AnimationTrackPtr> animTracks;
    vector<AnimationStackPtr> animStacks;

    bool looping;
    float power;

    size_t currentStackIndex;
    bool isUseAnimStack;

    Long currTime;
    Long beginTime;
};

class KeyFrame
{
public:
    KeyFrame();
    KeyFrame(const long time, const Vec3& tran, const Quat& rotation, const Vec3& scale);

    string getName() const;
    Long getTime() const;
    const Vec3& getTranslation() const;
    const Vec3& getScaling() const;
    const Quat& getRotation() const;
    const Mat4& getMatrix() const;

    void setName(const string &value);
    void setTranslation(const Vec3 &value);
    void setScaling(const Vec3 &value);
    void setRotation(const Quat &value);

private:
    string name;
    Long time;
    Vec3 translation;
    Vec3 scaling;
    Quat rotation;
    Mat4 matrix;
};

/*
 * one track on bone
 */
class AnimationTrack
{
    friend class Animation;
    friend class Bone;

public:
    void addKeyFrame(const KeyFrame& frame);

    Int getCurrKeyFrameIndex();
    Int getKeyFrameCount();

    KeyFramePtr getKeyFrame(int index);

    void updateTimeInfo();
    void updateTrackInfo();

    void updateLocalMatrix();

private:
    void calcProportion(Long timePos);
    void linearDeformation();

private:
    weak_ptr<Animation> animation;

    vector<shared_ptr<KeyFrame>> keyFrames;
    Long minKeyTime, maxKeyTime, animLength;

    float frameProportion;
    Int currentFrameIndex;

    shared_ptr<KeyFrame> interpolationBeginKeyFrame, interpolationEndKeyFrame;
    std::weak_ptr<Node> node;

    Long currentTime, beginTime;
};

/**
 * @brief The AnimationStack class
 *
 * Part of timeline, some action of one people, run, for example
 */
class AnimationStack
{
    friend class Animation;
    friend class AnimationTrack;

public:
    AnimationStack(Long begin, Long end);

    Long getStackBeginTime() const;
    Long getStackEndTime() const;

private:
    weak_ptr<Animation> animation;

    Long stackBeginTime;
    Long stackEndTime;
    Long stackLength;
};

} // namespace re

#endif // ANIMATION_H
