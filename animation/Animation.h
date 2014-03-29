#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include <memory>
#include "platform.h"

#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"

namespace re {

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

class AnimationTrack
{
public:
    void addKeyFrame(const KeyFrame& frame);
    Mat4 getLocalMatrix();

    Int getCurrKeyFrameIndex();
    Int getKeyFrameCount();

private:
    void calcProportion(Long timePos);
    Mat4 linearDeformation();

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

} // namespace re

#endif // ANIMATION_H
