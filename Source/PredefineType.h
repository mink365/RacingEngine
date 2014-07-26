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

class RenderTarget;
class Renderer;
class Material;
class Pass;
class Texture;
class TextureFrame;

class Camera;
class Light;

class File;
class Buffer;

class Sprite;
class Label;
class NinePatch;

class Widget;

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

typedef std::shared_ptr<Renderer> RendererPtr;
typedef std::shared_ptr<RenderTarget> RenderTargetPtr;
typedef std::shared_ptr<Material> MaterialPtr;
typedef std::shared_ptr<Pass> PassPtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<TextureFrame> TextureFramePtr;

typedef std::shared_ptr<Camera> CameraPtr;
typedef std::shared_ptr<Light> LightPtr;

typedef std::shared_ptr<File> FilePtr;
typedef std::shared_ptr<const File> ConstFilePtr;
typedef std::shared_ptr<Buffer> BufferPtr;
typedef std::shared_ptr<const Buffer> ConstBufferPtr;

typedef std::shared_ptr<Sprite> SpritePtr;
typedef std::shared_ptr<NinePatch> NinePatchPtr;
typedef std::shared_ptr<Label> LabelPtr;

typedef std::shared_ptr<Widget> WidgetPtr;

}

#endif // PREDEFINETYPE_H
