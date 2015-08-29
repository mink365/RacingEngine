#ifndef RE_ANIMATIONTRACK_H
#define RE_ANIMATIONTRACK_H

#include <memory>
#include <vector>
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

#include "Base/ECS/Component.h"

namespace re {

class Animation;
class AnimationTrack;
class KeyFrame;

typedef SharedPtr<AnimationTrack> AnimationTrackPtr;
typedef SharedPtr<KeyFrame> KeyFramePtr;

class KeyFrame
{
public:
    KeyFrame();
    KeyFrame(const long time, const Vec3& tran, const Quat& rotation, const Vec3& scale);

    std::string getName() const;
    Long getTime() const;
    const Vec3& getTranslation() const;
    const Vec3& getScaling() const;
    const Quat& getRotation() const;
    const Mat4& getMatrix() const;

    void setName(const std::string &value);
    void setTranslation(const Vec3 &value);
    void setScaling(const Vec3 &value);
    void setRotation(const Quat &value);

private:
    std::string name;
    Long time;
    Vec3 translation;
    Vec3 scaling;
    Quat rotation;
    Mat4 matrix;
};

/*
 * one track on bone
 * AnimationTrack will reference a bone, but it is't a
 * component of bone node
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
    WeakPtr<Animation> animation;
    WeakPtr<Entity> entity;

    std::vector<KeyFramePtr> keyFrames;
    Long minKeyTime, maxKeyTime, animLength;

    float frameProportion;
    Int currentFrameIndex;

    KeyFramePtr interpolationBeginKeyFrame, interpolationEndKeyFrame;

    Long currentTime, beginTime;
};


} // namespace re

#endif // RE_ANIMATIONTRACK_H
