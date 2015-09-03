#ifndef PREDECLARE_H
#define PREDECLARE_H

#include <memory>
#include "Base/Shared.h"
#include "Base/ECS/ComponentHandle.h"
#include "Resource/ResourceManager.h"

namespace re {

class AnimationTrack;
class AnimationStack;
class Animation;
class KeyFrame;
class Skeleton;
class SkeletonController;

class Entity;
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
class Image;
class Text;

class Widget;
class Button;

class LayoutElement;

}

typedef ComponentHandle<Animation> AnimationPtr;
typedef SharedPtr<AnimationTrack> AnimationTrackPtr;
typedef SharedPtr<AnimationStack> AnimationStackPtr;
typedef SharedPtr<KeyFrame> KeyFramePtr;
typedef ComponentHandle<Skeleton> SkeletonPtr;
typedef ComponentHandle<SkeletonController> SkeletonControllerPtr;

typedef SharedPtr<Entity> EntityPtr;
typedef ComponentHandle<Node> NodePtr;
typedef ComponentHandle<Bone> BonePtr;
typedef ComponentHandle<RenderElement> RenderElementPtr;
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

typedef ComponentHandle<Transform> TransformPtr;
typedef ComponentHandle<Camera> CameraPtr;
typedef ComponentHandle<Light> LightPtr;

typedef SharedPtr<File> FilePtr;
typedef SharedPtr<const File> ConstFilePtr;

typedef SharedPtr<Font> FontPtr;

namespace ui {

typedef ComponentHandle<Transform2D> Transform2DPtr;
typedef ComponentHandle<HierarchyColor> HierarchyColorPtr;
typedef ComponentHandle<Image> ImagePtr;
typedef ComponentHandle<Text> TextPtr;

typedef ComponentHandle<Widget> WidgetPtr;
typedef ComponentHandle<Button> ButtonPtr;

}

// define the ResourceManageres
typedef ResourceManager<Shader> ShaderManager;
typedef ResourceManager<Material> MaterialManager;
typedef ResourceManager<TextureFrame> TextureFrameManager;
typedef ResourceManager<Font> FontManager;

}

#endif // PREDEFINETYPE_H
