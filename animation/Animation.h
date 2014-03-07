#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include <memory>
#include "platform.h"

class reVec3;
class reQuat;
class reMat4;
class BoneNode;
class AnimationTrack;
class AnimationStack;

class Animation
{
public:
    Animation();

private:

    vector<shared_ptr<AnimationTrack>> animTracks;
    vector<shared_ptr<AnimationStack>> animStacks;

    bool looping;
    float power;

    Int currentStackIndex;
    bool isUseAnimStack;
};

class KeyFrame
{
public:
    KeyFrame();
    KeyFrame(const long time, const reVec3& tran, const reQuat& rotation, const reVec3& scale);

    string getName() const;
    Long getTime() const;
    const reVec3& getTranslation() const;
    const reVec3& getScaling() const;
    const reQuat& getRotation() const;
    const reMat4& getMatrix() const;

    void setName(const string &value);
    void setTranslation(const reVec3 &value);
    void setScaling(const reVec3 &value);
    void setRotation(const reQuat &value);

private:
    string name;
    Long time;
    reVec3 translation;
    reVec3 scaling;
    reQuat rotation;
    reMat4 matrix;
};

class AnimationTrack
{
public:
    void addKeyFrame(const KeyFrame& frame);
    reMat4 getLocalMatrix();

    Int getCurrKeyFrameIndex();
    Int getKeyFrameCount();

private:
    void calcProportion(Long timePos);
    reMat4 linearDeformation();

    void update();
private:
    weak_ptr<Animation> animation;

    vector<shared_ptr<KeyFrame>> keyFrames;
    Long minKeyTime, maxKeyTime, animLength;

    float frameProportion;
    Int currentFrameIndex;

    shared_ptr<KeyFrame> interpolationBeginKeyFrame, interpolationEndKeyFrame;
    shared_ptr<BoneNode> boneNode;

    Long currentTime, beginTime;
};

/**
 * @brief The AnimationStack class
 *
 * Part of timeline, some action of one people, run, for example
 */
class AnimationStack
{
public:
    AnimationStack(Long begin, Long end);

private:
    weak_ptr<Animation> animation;

    Long stackBeginTime;
    Long stackEndTime;
    Long stackLength;
};

#endif // ANIMATION_H
