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
    Long getEndTime() const;
    Long getBeginTime() const;
    Long getLength() const;
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
    Long length;
    Vec3 translation;
    Vec3 scaling;
    Quat rotation;
    Mat4 matrix;
};

class AnimationTrack
{
    friend class Animation;

public:
    void addKeyFrame(const KeyFrame& frame);
    Mat4 getLocalMatrix();

    Int getCurrKeyFrameIndex();
    Int getKeyFrameCount();

private:
    void calcProportion(Long timePos);
    Mat4 linearDeformation();

    void updateTrackInfo();
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
    friend class Animation;

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
