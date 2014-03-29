#ifndef PREDEFINETYPE_H
#define PREDEFINETYPE_H

#include <memory>

namespace re {

class AnimationTrack;
class AnimationStack;
class Animation;
class KeyFrame;

typedef std::shared_ptr<Animation> AnimationPtr;
typedef std::shared_ptr<AnimationTrack> AnimationTrackPtr;
typedef std::shared_ptr<AnimationStack> AnimationStackPtr;
typedef std::shared_ptr<KeyFrame> KeyFramePtr;

class BoneNode;
typedef std::shared_ptr<BoneNode> BoneNodePtr;

}

#endif // PREDEFINETYPE_H
