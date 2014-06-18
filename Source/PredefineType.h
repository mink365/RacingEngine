#ifndef PREDEFINETYPE_H
#define PREDEFINETYPE_H

#include <memory>

namespace re {

class AnimationTrack;
class AnimationStack;
class Animation;
class KeyFrame;
class Skeleton;
class SkeletonController;

class BoneNode;
class SceneNode;
class NodeAttribute;
class Mesh;

typedef std::shared_ptr<Animation> AnimationPtr;
typedef std::shared_ptr<AnimationTrack> AnimationTrackPtr;
typedef std::shared_ptr<AnimationStack> AnimationStackPtr;
typedef std::shared_ptr<KeyFrame> KeyFramePtr;
typedef std::shared_ptr<Skeleton> SkeletonPtr;
typedef std::shared_ptr<SkeletonController> SkeletonControllerPtr;

typedef std::shared_ptr<BoneNode> BoneNodePtr;
typedef std::shared_ptr<SceneNode> SceneNodePtr;
typedef std::shared_ptr<NodeAttribute> NodeAttributePtr;
typedef std::shared_ptr<Mesh> MeshPtr;

}

#endif // PREDEFINETYPE_H
