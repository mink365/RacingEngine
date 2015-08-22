#ifndef PREDECLARE_H
#define PREDECLARE_H

#include <memory>
#include "Base/Buffer.h"
#include "Resource/ResourceManager.h"

namespace re {

template<typename T>
using SharedPtr = std::shared_ptr<T>;

class AnimationTrack;
class AnimationStack;
class Animation;
class KeyFrame;
class Skeleton;
class SkeletonController;

class Entity;
class Component;
class Node;
class Bone;
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
class TextureAtlas;

class Transform;
class Camera;
class Light;

class File;
class Font;

namespace ui {

class Transform2D;
class HierarchyColor;
class Sprite;
class Label;
class NinePatch;

class Widget;
class BaseButton;

class LayoutElement;

}

typedef SharedPtr<Animation> AnimationPtr;
typedef SharedPtr<AnimationTrack> AnimationTrackPtr;
typedef SharedPtr<AnimationStack> AnimationStackPtr;
typedef SharedPtr<KeyFrame> KeyFramePtr;
typedef SharedPtr<Skeleton> SkeletonPtr;
typedef SharedPtr<SkeletonController> SkeletonControllerPtr;

typedef SharedPtr<Entity> EntityPtr;
typedef SharedPtr<Component> ComponentPtr;
typedef SharedPtr<Node> NodePtr;
typedef SharedPtr<Bone> BonePtr;
typedef SharedPtr<RenderElement> RenderElementPtr;
typedef SharedPtr<Mesh> MeshPtr;
typedef SharedPtr<MeshData> MeshDataPtr;
typedef SharedPtr<SkinnedMeshData> SkinnedMeshDataPtr;
typedef SharedPtr<Geometry> GeometryPtr;

typedef SharedPtr<Renderer> RendererPtr;
typedef SharedPtr<RenderTarget> RenderTargetPtr;
typedef SharedPtr<Material> MaterialPtr;
typedef SharedPtr<Shader> ShaderPtr;
typedef SharedPtr<Pass> PassPtr;
typedef SharedPtr<Texture> TexturePtr;
typedef SharedPtr<TextureFrame> TextureFramePtr;
typedef SharedPtr<TextureAtlas> TextureAtlasPtr;

typedef SharedPtr<Transform> TransformPtr;
typedef SharedPtr<Camera> CameraPtr;
typedef SharedPtr<Light> LightPtr;

typedef SharedPtr<File> FilePtr;
typedef SharedPtr<const File> ConstFilePtr;

typedef SharedPtr<Font> FontPtr;

typedef SharedPtr<ByteBuffer> ByteBufferPtr;

namespace ui {

typedef SharedPtr<Transform2D> Transform2DPtr;
typedef SharedPtr<HierarchyColor> HierarchyColorPtr;
typedef SharedPtr<Sprite> SpritePtr;
typedef SharedPtr<NinePatch> NinePatchPtr;
typedef SharedPtr<Label> LabelPtr;

typedef SharedPtr<Widget> WidgetPtr;
typedef SharedPtr<BaseButton> ButtonPtr;

}

// define the ResourceManageres
typedef ResourceManager<Shader> ShaderManager;
typedef ResourceManager<Material> MaterialManager;
typedef ResourceManager<TextureFrame> TextureFrameManager;
typedef ResourceManager<Font> FontManager;

}

#endif // PREDEFINETYPE_H
