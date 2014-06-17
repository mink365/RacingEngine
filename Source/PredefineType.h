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

typedef std::shared_ptr<Animation> AnimationPtr;
typedef std::shared_ptr<AnimationTrack> AnimationTrackPtr;
typedef std::shared_ptr<AnimationStack> AnimationStackPtr;
typedef std::shared_ptr<KeyFrame> KeyFramePtr;
typedef std::shared_ptr<Skeleton> SkeletonPtr;
typedef std::shared_ptr<SkeletonController> SkeletonControllerPtr;

class BoneNode;
typedef std::shared_ptr<BoneNode> BoneNodePtr;

class SceneNode;
typedef std::shared_ptr<SceneNode> SceneNodePtr;

class NodeAttribute;
typedef std::shared_ptr<NodeAttribute> NodeAttributePtr;

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

}

#endif // PREDEFINETYPE_H
