#ifndef PREDECLARE_H
#define PREDECLARE_H

#include <memory>
#include "Base/Buffer.h"
#include "Resource/ResourceManager.h"

namespace re {

class AnimationTrack;
class AnimationStack;
class Animation;
class KeyFrame;
class Skeleton;
class SkeletonController;

class Bone;
class Node;
class Component;
class RenderElement;
class Mesh;
class MeshData;
class SkinnedMeshData;
class Geometry;

class RenderTarget;
class Renderer;
class Material;
class Shader;
class Pass;
class Texture;
class TextureFrame;

class Transform;
class Camera;
class Light;

class File;
class Font;

class Transform2D;
class HierarchyColor;
class Sprite;
class Label;
class NinePatch;

class Widget;
class BaseButton;

typedef std::shared_ptr<Animation> AnimationPtr;
typedef std::shared_ptr<AnimationTrack> AnimationTrackPtr;
typedef std::shared_ptr<AnimationStack> AnimationStackPtr;
typedef std::shared_ptr<KeyFrame> KeyFramePtr;
typedef std::shared_ptr<Skeleton> SkeletonPtr;
typedef std::shared_ptr<SkeletonController> SkeletonControllerPtr;

typedef std::shared_ptr<Bone> BonePtr;
typedef std::shared_ptr<Node> NodePtr;
typedef std::shared_ptr<Component> ComponentPtr;
typedef std::shared_ptr<RenderElement> RenderElementPtr;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<MeshData> MeshDataPtr;
typedef std::shared_ptr<SkinnedMeshData> SkinnedMeshDataPtr;
typedef std::shared_ptr<Geometry> GeometryPtr;

typedef std::shared_ptr<Renderer> RendererPtr;
typedef std::shared_ptr<RenderTarget> RenderTargetPtr;
typedef std::shared_ptr<Material> MaterialPtr;
typedef std::shared_ptr<Pass> PassPtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<TextureFrame> TextureFramePtr;

typedef std::shared_ptr<Transform> TransformPtr;
typedef std::shared_ptr<Camera> CameraPtr;
typedef std::shared_ptr<Light> LightPtr;

typedef std::shared_ptr<File> FilePtr;
typedef std::shared_ptr<const File> ConstFilePtr;

typedef std::shared_ptr<Font> FontPtr;

typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;

typedef std::shared_ptr<Transform2D> Transform2DPtr;
typedef std::shared_ptr<HierarchyColor> HierarchyColorPtr;
typedef std::shared_ptr<Sprite> SpritePtr;
typedef std::shared_ptr<NinePatch> NinePatchPtr;
typedef std::shared_ptr<Label> LabelPtr;

typedef std::shared_ptr<Widget> WidgetPtr;
typedef std::shared_ptr<BaseButton> ButtonPtr;

// define the ResourceManageres
typedef ResourceManager<Shader> ShaderManager;
typedef ResourceManager<Material> MaterialManager;
typedef ResourceManager<TextureFrame> TextureFrameManager;
typedef ResourceManager<Font> FontManager;

}

#endif // PREDEFINETYPE_H
