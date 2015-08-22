#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include <memory>
#include "platform.h"
#include "Scene/Component.h"

namespace re {

class AnimationStack;
class Animation;

typedef SharedPtr<Animation> AnimationPtr;
typedef SharedPtr<AnimationStack> AnimationStackPtr;

class Animation : public Component, public std::enable_shared_from_this<Animation>
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

    std::vector<AnimationTrackPtr> animTracks;
    std::vector<AnimationStackPtr> animStacks;

    bool looping;
    float power;

    size_t currentStackIndex;
    bool isUseAnimStack;

    Long currTime;
    Long beginTime;
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
    std::weak_ptr<Animation> animation;

    Long stackBeginTime;
    Long stackEndTime;
    Long stackLength;
};

} // namespace re

#endif // ANIMATION_H
