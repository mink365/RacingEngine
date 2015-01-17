#ifndef RE_ANIMATIONTRACK_H
#define RE_ANIMATIONTRACK_H

#include <memory>
#include <vector>
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

namespace re {

class AnimationTrack;
class KeyFrame;

typedef shared_ptr<AnimationTrack> AnimationTrackPtr;
typedef shared_ptr<KeyFrame> KeyFramePtr;

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


} // namespace re

#endif // RE_ANIMATIONTRACK_H
