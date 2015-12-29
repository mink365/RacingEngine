

set(PRE_HEADER_FILES
        ${RE_SOURCE_DIR}/PreDeclare.h
        ${RE_SOURCE_DIR}/opengl.h
        ${RE_SOURCE_DIR}/platform.h
        )
	
set(ANIMATION_SOURCE_FILES 
        ${RE_SOURCE_DIR}/Animation/Animation.cpp
        ${RE_SOURCE_DIR}/Animation/AnimationTrack.cpp
        ${RE_SOURCE_DIR}/Animation/Bone.cpp
        ${RE_SOURCE_DIR}/Animation/Skeleton.cpp
        ${RE_SOURCE_DIR}/Animation/SkeletonController.cpp
        )

set(ANIMATION_HEADER_FILES 
        ${RE_SOURCE_DIR}/Animation/Animation.h 
        ${RE_SOURCE_DIR}/Animation/AnimationTrack.h 
        ${RE_SOURCE_DIR}/Animation/Bone.h 
        ${RE_SOURCE_DIR}/Animation/Skeleton.h 
        ${RE_SOURCE_DIR}/Animation/SkeletonController.h 
        )

SOURCE_GROUP("Animation\\Source Files" FILES ${ANIMATION_SOURCE_FILES})
SOURCE_GROUP("Animation\\Header Files" FILES ${ANIMATION_HEADER_FILES})
 
	
set(AUDIO_SOURCE_FILES 
        ${RE_SOURCE_DIR}/Audio/AudioEngine.cpp
        ${RE_SOURCE_DIR}/Audio/AudioListener.cpp
        ${RE_SOURCE_DIR}/Audio/AudioSource.cpp
        ${RE_SOURCE_DIR}/Audio/OpenAL/OpenALAudioEngine.cpp
        ${RE_SOURCE_DIR}/Audio/OpenAL/OpenALAudioFactory.cpp
        ${RE_SOURCE_DIR}/Audio/OpenAL/OpenALBuffer.cpp
        ${RE_SOURCE_DIR}/Audio/OpenAL/OpenALListener.cpp
        ${RE_SOURCE_DIR}/Audio/OpenAL/OpenALSource.cpp
        )

set(AUDIO_HEADER_FILES 
        ${RE_SOURCE_DIR}/Audio/AudioEngine.h 
        ${RE_SOURCE_DIR}/Audio/AudioListener.h 
        ${RE_SOURCE_DIR}/Audio/AudioSource.h 
        ${RE_SOURCE_DIR}/Audio/OpenAL/OpenALAudioEngine.h 
        ${RE_SOURCE_DIR}/Audio/OpenAL/OpenALAudioFactory.h 
        ${RE_SOURCE_DIR}/Audio/OpenAL/OpenALBase.h 
        ${RE_SOURCE_DIR}/Audio/OpenAL/OpenALBuffer.h 
        ${RE_SOURCE_DIR}/Audio/OpenAL/OpenALListener.h 
        ${RE_SOURCE_DIR}/Audio/OpenAL/OpenALSource.h 
        )

SOURCE_GROUP("Audio\\Source Files" FILES ${AUDIO_SOURCE_FILES})
SOURCE_GROUP("Audio\\Header Files" FILES ${AUDIO_HEADER_FILES})
 
	
set(BASE_SOURCE_FILES 
        ${RE_SOURCE_DIR}/Base/ECS/Component.cpp
        ${RE_SOURCE_DIR}/Base/ECS/Dependencies.cpp
        ${RE_SOURCE_DIR}/Base/ECS/Entity.cpp
        ${RE_SOURCE_DIR}/Base/ECS/EntityManager.cpp
        ${RE_SOURCE_DIR}/Base/ECS/Event.cpp
        ${RE_SOURCE_DIR}/Base/ECS/System.cpp
        ${RE_SOURCE_DIR}/Base/Memory/Buffer.cpp
        )

set(BASE_HEADER_FILES 
        ${RE_SOURCE_DIR}/Base/Clonable.h 
        ${RE_SOURCE_DIR}/Base/ECS/Component.h 
        ${RE_SOURCE_DIR}/Base/ECS/ComponentHandle.h 
        ${RE_SOURCE_DIR}/Base/ECS/Dependencies.h 
        ${RE_SOURCE_DIR}/Base/ECS/Entity.h 
        ${RE_SOURCE_DIR}/Base/ECS/EntityManager.h 
        ${RE_SOURCE_DIR}/Base/ECS/Event.h 
        ${RE_SOURCE_DIR}/Base/ECS/System.h 
        ${RE_SOURCE_DIR}/Base/Memory/Buffer.h 
        ${RE_SOURCE_DIR}/Base/Named.h 
        ${RE_SOURCE_DIR}/Base/NamedFactory.h 
        ${RE_SOURCE_DIR}/Base/Shared.h 
        ${RE_SOURCE_DIR}/Base/Singleton.h 
        ${RE_SOURCE_DIR}/Base/Uncopyable.h 
        )

SOURCE_GROUP("Base\\Source Files" FILES ${BASE_SOURCE_FILES})
SOURCE_GROUP("Base\\Header Files" FILES ${BASE_HEADER_FILES})
 
	
set(EFFECT_SOURCE_FILES 
        ${RE_SOURCE_DIR}/Effect/Particle/ParticleSystem.cpp
        )

set(EFFECT_HEADER_FILES 
        ${RE_SOURCE_DIR}/Effect/Particle/ParticleSystem.h 
        )

SOURCE_GROUP("Effect\\Source Files" FILES ${EFFECT_SOURCE_FILES})
SOURCE_GROUP("Effect\\Header Files" FILES ${EFFECT_HEADER_FILES})
 
	
set(FILESYSTEM_SOURCE_FILES 
        ${RE_SOURCE_DIR}/FileSystem/File.cpp
        ${RE_SOURCE_DIR}/FileSystem/FileSystem.cpp
        )

set(FILESYSTEM_HEADER_FILES 
        ${RE_SOURCE_DIR}/FileSystem/File.h 
        ${RE_SOURCE_DIR}/FileSystem/FileSystem.h 
        )

SOURCE_GROUP("FileSystem\\Source Files" FILES ${FILESYSTEM_SOURCE_FILES})
SOURCE_GROUP("FileSystem\\Header Files" FILES ${FILESYSTEM_HEADER_FILES})
 
	
set(FONT_SOURCE_FILES 
        ${RE_SOURCE_DIR}/Font/Font.cpp
        ${RE_SOURCE_DIR}/Font/FreeTypeUtil.cpp
        ${RE_SOURCE_DIR}/Font/Glyph.cpp
        ${RE_SOURCE_DIR}/Font/TextStuffer.cpp
        ${RE_SOURCE_DIR}/Font/TextureAtlas.cpp
        )

set(FONT_HEADER_FILES 
        ${RE_SOURCE_DIR}/Font/Font.h 
        ${RE_SOURCE_DIR}/Font/FreeTypeUtil.h 
        ${RE_SOURCE_DIR}/Font/Glyph.h 
        ${RE_SOURCE_DIR}/Font/TextStuffer.h 
        ${RE_SOURCE_DIR}/Font/TextureAtlas.h 
        )

SOURCE_GROUP("Font\\Source Files" FILES ${FONT_SOURCE_FILES})
SOURCE_GROUP("Font\\Header Files" FILES ${FONT_HEADER_FILES})
 
	
set(MATH_SOURCE_FILES 
        ${RE_SOURCE_DIR}/Math/AABBox.cpp
        ${RE_SOURCE_DIR}/Math/Box.cpp
        ${RE_SOURCE_DIR}/Math/Color.cpp
        ${RE_SOURCE_DIR}/Math/Frustum.cpp
        ${RE_SOURCE_DIR}/Math/Math.cpp
        ${RE_SOURCE_DIR}/Math/Matrix.cpp
        ${RE_SOURCE_DIR}/Math/OBBox.cpp
        ${RE_SOURCE_DIR}/Math/Plane.cpp
        ${RE_SOURCE_DIR}/Math/Quaternion.cpp
        ${RE_SOURCE_DIR}/Math/Ray.cpp
        ${RE_SOURCE_DIR}/Math/Rect.cpp
        ${RE_SOURCE_DIR}/Math/RectOffset.cpp
        ${RE_SOURCE_DIR}/Math/Size.cpp
        ${RE_SOURCE_DIR}/Math/Sphere.cpp
        ${RE_SOURCE_DIR}/Math/Time.cpp
        ${RE_SOURCE_DIR}/Math/UV.cpp
        ${RE_SOURCE_DIR}/Math/Vector.cpp
        )

set(MATH_HEADER_FILES 
        ${RE_SOURCE_DIR}/Math/AABBox.h 
        ${RE_SOURCE_DIR}/Math/Box.h 
        ${RE_SOURCE_DIR}/Math/Color.h 
        ${RE_SOURCE_DIR}/Math/Frustum.h 
        ${RE_SOURCE_DIR}/Math/Math.h 
        ${RE_SOURCE_DIR}/Math/MathUtil.h 
        ${RE_SOURCE_DIR}/Math/Matrix.h 
        ${RE_SOURCE_DIR}/Math/OBBox.h 
        ${RE_SOURCE_DIR}/Math/Plane.h 
        ${RE_SOURCE_DIR}/Math/Quaternion.h 
        ${RE_SOURCE_DIR}/Math/Ray.h 
        ${RE_SOURCE_DIR}/Math/Rect.h 
        ${RE_SOURCE_DIR}/Math/RectOffset.h 
        ${RE_SOURCE_DIR}/Math/Size.h 
        ${RE_SOURCE_DIR}/Math/Sphere.h 
        ${RE_SOURCE_DIR}/Math/Time.h 
        ${RE_SOURCE_DIR}/Math/UV.h 
        ${RE_SOURCE_DIR}/Math/Vector.h 
        )

SOURCE_GROUP("Math\\Source Files" FILES ${MATH_SOURCE_FILES})
SOURCE_GROUP("Math\\Header Files" FILES ${MATH_HEADER_FILES})
 
	
set(MESSAGE_SOURCE_FILES 
        ${RE_SOURCE_DIR}/Message/Message.cpp
        ${RE_SOURCE_DIR}/Message/MessageConstant.cpp
        ${RE_SOURCE_DIR}/Message/MessageManager.cpp
        ${RE_SOURCE_DIR}/Message/Signal.cpp
        )

set(MESSAGE_HEADER_FILES 
        ${RE_SOURCE_DIR}/Message/Message.h 
        ${RE_SOURCE_DIR}/Message/MessageConstant.h 
        ${RE_SOURCE_DIR}/Message/MessageManager.h 
        ${RE_SOURCE_DIR}/Message/Signal.h 
        )

SOURCE_GROUP("Message\\Source Files" FILES ${MESSAGE_SOURCE_FILES})
SOURCE_GROUP("Message\\Header Files" FILES ${MESSAGE_HEADER_FILES})
 
	
set(RENDER_SOURCE_FILES 
        ${RE_SOURCE_DIR}/Render/BufferObject/BufferObjectUtil.cpp
        ${RE_SOURCE_DIR}/Render/BufferObject/IndexBuffer.cpp
        ${RE_SOURCE_DIR}/Render/BufferObject/VertexBuffer.cpp
        ${RE_SOURCE_DIR}/Render/Geometry/Geometry.cpp
        ${RE_SOURCE_DIR}/Render/Material/Material.cpp
        ${RE_SOURCE_DIR}/Render/Material/Pass.cpp
        ${RE_SOURCE_DIR}/Render/Material/SamplerParameter.cpp
        ${RE_SOURCE_DIR}/Render/Mesh.cpp
        ${RE_SOURCE_DIR}/Render/Renderable.cpp
        ${RE_SOURCE_DIR}/Render/RenderContext.cpp
        ${RE_SOURCE_DIR}/Render/Renderer/GLES2Renderer.cpp
        ${RE_SOURCE_DIR}/Render/Renderer/Renderer.cpp
        ${RE_SOURCE_DIR}/Render/RenderManager.cpp
        ${RE_SOURCE_DIR}/Render/RenderQueue.cpp
        ${RE_SOURCE_DIR}/Render/RenderState.cpp
        ${RE_SOURCE_DIR}/Render/RenderTarget.cpp
        ${RE_SOURCE_DIR}/Render/RenderView.cpp
        ${RE_SOURCE_DIR}/Render/Shader/Attribute.cpp
        ${RE_SOURCE_DIR}/Render/Shader/Shader.cpp
        ${RE_SOURCE_DIR}/Render/Shader/ShaderUtil.cpp
        ${RE_SOURCE_DIR}/Render/Shader/Uniform.cpp
        ${RE_SOURCE_DIR}/Render/Texture/Frame/TextureFrame.cpp
        ${RE_SOURCE_DIR}/Render/Texture/PVRTextureUtil.cpp
        ${RE_SOURCE_DIR}/Render/Texture/PVRTTexture.cpp
        ${RE_SOURCE_DIR}/Render/Texture/Texture.cpp
        ${RE_SOURCE_DIR}/Render/Texture/TextureManager.cpp
        ${RE_SOURCE_DIR}/Render/Texture/TextureParser.cpp
        ${RE_SOURCE_DIR}/Render/Texture/TextureUtil.cpp
        )

set(RENDER_HEADER_FILES 
        ${RE_SOURCE_DIR}/Render/BufferObject/BufferObjectUtil.h 
        ${RE_SOURCE_DIR}/Render/BufferObject/IndexBuffer.h 
        ${RE_SOURCE_DIR}/Render/BufferObject/VertexBuffer.h 
        ${RE_SOURCE_DIR}/Render/Geometry/Geometry.h 
        ${RE_SOURCE_DIR}/Render/Material/Material.h 
        ${RE_SOURCE_DIR}/Render/Material/Pass.h 
        ${RE_SOURCE_DIR}/Render/Material/SamplerParameter.h 
        ${RE_SOURCE_DIR}/Render/Mesh.h 
        ${RE_SOURCE_DIR}/Render/Renderable.h 
        ${RE_SOURCE_DIR}/Render/RenderContext.h 
        ${RE_SOURCE_DIR}/Render/Renderer/GLES2Renderer.h 
        ${RE_SOURCE_DIR}/Render/Renderer/Renderer.h 
        ${RE_SOURCE_DIR}/Render/RenderManager.h 
        ${RE_SOURCE_DIR}/Render/RenderQueue.h 
        ${RE_SOURCE_DIR}/Render/RenderState.h 
        ${RE_SOURCE_DIR}/Render/RenderTarget.h 
        ${RE_SOURCE_DIR}/Render/RenderView.h 
        ${RE_SOURCE_DIR}/Render/Shader/Attribute.h 
        ${RE_SOURCE_DIR}/Render/Shader/Shader.h 
        ${RE_SOURCE_DIR}/Render/Shader/ShaderUtil.h 
        ${RE_SOURCE_DIR}/Render/Shader/Uniform.h 
        ${RE_SOURCE_DIR}/Render/Texture/Frame/TextureFrame.h 
        ${RE_SOURCE_DIR}/Render/Texture/gl2extimg.h 
        ${RE_SOURCE_DIR}/Render/Texture/PVRTextureUtil.h 
        ${RE_SOURCE_DIR}/Render/Texture/PVRTTexture.h 
        ${RE_SOURCE_DIR}/Render/Texture/Texture.h 
        ${RE_SOURCE_DIR}/Render/Texture/TextureManager.h 
        ${RE_SOURCE_DIR}/Render/Texture/TextureParser.h 
        ${RE_SOURCE_DIR}/Render/Texture/TextureUtil.h 
        )

SOURCE_GROUP("Render\\Source Files" FILES ${RENDER_SOURCE_FILES})
SOURCE_GROUP("Render\\Header Files" FILES ${RENDER_HEADER_FILES})
 
	
set(RESOURCE_SOURCE_FILES 
        ${RE_SOURCE_DIR}/Resource/AtlasParser/BaseAtlasParser.cpp
        ${RE_SOURCE_DIR}/Resource/AtlasParser/JSONAtlasParser.cpp
        ${RE_SOURCE_DIR}/Resource/BitmapFontParser/AngleFontParser.cpp
        ${RE_SOURCE_DIR}/Resource/BitmapFontParser/BaseBitmapFontParser.cpp
        ${RE_SOURCE_DIR}/Resource/BitmapFontParser/BitmapFontFactory.cpp
        ${RE_SOURCE_DIR}/Resource/Common/BlockParser.cpp
        ${RE_SOURCE_DIR}/Resource/Loader/MaterialLoader.cpp
        ${RE_SOURCE_DIR}/Resource/ModelLoader/fbx/FbxParser.cpp
        ${RE_SOURCE_DIR}/Resource/ModelLoader/ReadCommon.cpp
        ${RE_SOURCE_DIR}/Resource/Pixmap/Pixmap.cpp
        )

set(RESOURCE_HEADER_FILES 
        ${RE_SOURCE_DIR}/Resource/AtlasParser/BaseAtlasParser.h 
        ${RE_SOURCE_DIR}/Resource/AtlasParser/JSONAtlasParser.h 
        ${RE_SOURCE_DIR}/Resource/BitmapFontParser/AngleFontParser.h 
        ${RE_SOURCE_DIR}/Resource/BitmapFontParser/BaseBitmapFontParser.h 
        ${RE_SOURCE_DIR}/Resource/BitmapFontParser/BitmapFontFactory.h 
        ${RE_SOURCE_DIR}/Resource/Common/BlockParser.h 
        ${RE_SOURCE_DIR}/Resource/Common/MemBuffer.h 
        ${RE_SOURCE_DIR}/Resource/Loader/MaterialLoader.h 
        ${RE_SOURCE_DIR}/Resource/ModelLoader/fbx/FbxParser.h 
        ${RE_SOURCE_DIR}/Resource/ModelLoader/ReadCommon.h 
        ${RE_SOURCE_DIR}/Resource/Pixmap/Pixmap.h 
        ${RE_SOURCE_DIR}/Resource/ResourceManager.h 
        )

SOURCE_GROUP("Resource\\Source Files" FILES ${RESOURCE_SOURCE_FILES})
SOURCE_GROUP("Resource\\Header Files" FILES ${RESOURCE_HEADER_FILES})
 
	
set(SCENE_SOURCE_FILES 
        ${RE_SOURCE_DIR}/Scene/Camera/Camera.cpp
        ${RE_SOURCE_DIR}/Scene/Light/DirectionalLight.cpp
        ${RE_SOURCE_DIR}/Scene/Light/Light.cpp
        ${RE_SOURCE_DIR}/Scene/Light/SpotLight.cpp
        ${RE_SOURCE_DIR}/Scene/Node.cpp
        ${RE_SOURCE_DIR}/Scene/RenderElement.cpp
        ${RE_SOURCE_DIR}/Scene/SceneManager.cpp
        ${RE_SOURCE_DIR}/Scene/Transform.cpp
        )

set(SCENE_HEADER_FILES 
        ${RE_SOURCE_DIR}/Scene/Camera/Camera.h 
        ${RE_SOURCE_DIR}/Scene/Light/DirectionalLight.h 
        ${RE_SOURCE_DIR}/Scene/Light/Light.h 
        ${RE_SOURCE_DIR}/Scene/Light/SpotLight.h 
        ${RE_SOURCE_DIR}/Scene/Node.h 
        ${RE_SOURCE_DIR}/Scene/RenderElement.h 
        ${RE_SOURCE_DIR}/Scene/SceneManager.h 
        ${RE_SOURCE_DIR}/Scene/Transform.h 
        )

SOURCE_GROUP("Scene\\Source Files" FILES ${SCENE_SOURCE_FILES})
SOURCE_GROUP("Scene\\Header Files" FILES ${SCENE_HEADER_FILES})
 
	
set(UI_SOURCE_FILES 
        ${RE_SOURCE_DIR}/UI/AnimView.cpp
        ${RE_SOURCE_DIR}/UI/Base/Graphic.cpp
        ${RE_SOURCE_DIR}/UI/Base/HierarchyColor.cpp
        ${RE_SOURCE_DIR}/UI/Base/Image.cpp
        ${RE_SOURCE_DIR}/UI/Base/Mask.cpp
        ${RE_SOURCE_DIR}/UI/Base/Maskable.cpp
        ${RE_SOURCE_DIR}/UI/Base/QuadStuffer.cpp
        ${RE_SOURCE_DIR}/UI/Base/Text.cpp
        ${RE_SOURCE_DIR}/UI/Base/Transform2D.cpp
        ${RE_SOURCE_DIR}/UI/Layout/GridLayoutGroup.cpp
        ${RE_SOURCE_DIR}/UI/Layout/HorizontalLayoutGroup.cpp
        ${RE_SOURCE_DIR}/UI/Layout/LayoutElement.cpp
        ${RE_SOURCE_DIR}/UI/Layout/LayoutGroup.cpp
        ${RE_SOURCE_DIR}/UI/Layout/LayoutUtil.cpp
        ${RE_SOURCE_DIR}/UI/Layout/LinearLayoutGroup.cpp
        ${RE_SOURCE_DIR}/UI/Layout/Screen.cpp
        ${RE_SOURCE_DIR}/UI/Layout/VerticalLayoutGroup.cpp
        ${RE_SOURCE_DIR}/UI/Manager/LayerManager.cpp
        ${RE_SOURCE_DIR}/UI/Manager/UIManager.cpp
        ${RE_SOURCE_DIR}/UI/Manager/WindowManager.cpp
        ${RE_SOURCE_DIR}/UI/Rendering/Canvas.cpp
        ${RE_SOURCE_DIR}/UI/Rendering/CanvasRenderElement.cpp
        ${RE_SOURCE_DIR}/UI/Rendering/DebugRenderer.cpp
        ${RE_SOURCE_DIR}/UI/Rendering/ElementBatch.cpp
        ${RE_SOURCE_DIR}/UI/Rendering/ElementBatcher.cpp
        ${RE_SOURCE_DIR}/UI/Rendering/FlatRenderer.cpp
        ${RE_SOURCE_DIR}/UI/Scene.cpp
        ${RE_SOURCE_DIR}/UI/TouchEvent.cpp
        ${RE_SOURCE_DIR}/UI/Widget/Button.cpp
        ${RE_SOURCE_DIR}/UI/Widget/ListView/AxisData.cpp
        ${RE_SOURCE_DIR}/UI/Widget/ListView/ListView.cpp
        ${RE_SOURCE_DIR}/UI/Widget/ListView/ScrollAnimation.cpp
        ${RE_SOURCE_DIR}/UI/Widget/ListView/ScrollView.cpp
        ${RE_SOURCE_DIR}/UI/Widget/ListView/VelocityTracker.cpp
        ${RE_SOURCE_DIR}/UI/Widget.cpp
        ${RE_SOURCE_DIR}/UI/Window.cpp
        )

set(UI_HEADER_FILES 
        ${RE_SOURCE_DIR}/UI/AnimView.h 
        ${RE_SOURCE_DIR}/UI/Base/Graphic.h 
        ${RE_SOURCE_DIR}/UI/Base/HierarchyColor.h 
        ${RE_SOURCE_DIR}/UI/Base/Image.h 
        ${RE_SOURCE_DIR}/UI/Base/Mask.h 
        ${RE_SOURCE_DIR}/UI/Base/Maskable.h 
        ${RE_SOURCE_DIR}/UI/Base/QuadStuffer.h 
        ${RE_SOURCE_DIR}/UI/Base/Text.h 
        ${RE_SOURCE_DIR}/UI/Base/Transform2D.h 
        ${RE_SOURCE_DIR}/UI/IBackKeyHandler.h 
        ${RE_SOURCE_DIR}/UI/Layout/GridLayoutGroup.h 
        ${RE_SOURCE_DIR}/UI/Layout/HorizontalLayoutGroup.h 
        ${RE_SOURCE_DIR}/UI/Layout/LayoutElement.h 
        ${RE_SOURCE_DIR}/UI/Layout/LayoutGroup.h 
        ${RE_SOURCE_DIR}/UI/Layout/LayoutUtil.h 
        ${RE_SOURCE_DIR}/UI/Layout/LinearLayoutGroup.h 
        ${RE_SOURCE_DIR}/UI/Layout/Screen.h 
        ${RE_SOURCE_DIR}/UI/Layout/VerticalLayoutGroup.h 
        ${RE_SOURCE_DIR}/UI/Manager/LayerManager.h 
        ${RE_SOURCE_DIR}/UI/Manager/UIManager.h 
        ${RE_SOURCE_DIR}/UI/Manager/WindowManager.h 
        ${RE_SOURCE_DIR}/UI/Rendering/Canvas.h 
        ${RE_SOURCE_DIR}/UI/Rendering/CanvasRenderElement.h 
        ${RE_SOURCE_DIR}/UI/Rendering/DebugRenderer.h 
        ${RE_SOURCE_DIR}/UI/Rendering/ElementBatch.h 
        ${RE_SOURCE_DIR}/UI/Rendering/ElementBatcher.h 
        ${RE_SOURCE_DIR}/UI/Rendering/FlatRenderer.h 
        ${RE_SOURCE_DIR}/UI/Scene.h 
        ${RE_SOURCE_DIR}/UI/TouchEvent.h 
        ${RE_SOURCE_DIR}/UI/Widget/Button.h 
        ${RE_SOURCE_DIR}/UI/Widget/ListView/AxisData.h 
        ${RE_SOURCE_DIR}/UI/Widget/ListView/ListView.h 
        ${RE_SOURCE_DIR}/UI/Widget/ListView/ScrollAnimation.h 
        ${RE_SOURCE_DIR}/UI/Widget/ListView/ScrollView.h 
        ${RE_SOURCE_DIR}/UI/Widget/ListView/VelocityTracker.h 
        ${RE_SOURCE_DIR}/UI/Widget.h 
        ${RE_SOURCE_DIR}/UI/Window.h 
        )

SOURCE_GROUP("UI\\Source Files" FILES ${UI_SOURCE_FILES})
SOURCE_GROUP("UI\\Header Files" FILES ${UI_HEADER_FILES})
 
	
set(UTIL_SOURCE_FILES 
        ${RE_SOURCE_DIR}/Util/ComponentFactory.cpp
        ${RE_SOURCE_DIR}/Util/EventUtil.cpp
        ${RE_SOURCE_DIR}/Util/Logging.cpp
        ${RE_SOURCE_DIR}/Util/PredefineTweenAccessor.cpp
        )

set(UTIL_HEADER_FILES 
        ${RE_SOURCE_DIR}/Util/ComponentFactory.h 
        ${RE_SOURCE_DIR}/Util/ContainerUtil.h 
        ${RE_SOURCE_DIR}/Util/EventUtil.h 
        ${RE_SOURCE_DIR}/Util/Logging.h 
        ${RE_SOURCE_DIR}/Util/PredefineTweenAccessor.h 
        ${RE_SOURCE_DIR}/Util/PrintUtil.h 
        )

SOURCE_GROUP("Util\\Source Files" FILES ${UTIL_SOURCE_FILES})
SOURCE_GROUP("Util\\Header Files" FILES ${UTIL_HEADER_FILES})

set(PLATFORM_SOURCE_FILES
        ${RE_SOURCE_DIR}/Platform/GameHub.cpp
        )

set(PLATFORM_HEADER_FILES
        ${RE_SOURCE_DIR}/Platform/ApplicationProtocol.h
        ${RE_SOURCE_DIR}/Platform/GameHub.h
        ${RE_SOURCE_DIR}/Platform/NativeView.h
        )

if(${RE_PLATFORM_MAC})
    set(PLATFORM_SPECIAL_FILES
            ${RE_SOURCE_DIR}/Platform/linux/Application.cpp
            ${RE_SOURCE_DIR}/Platform/linux/Application.h
            ${RE_SOURCE_DIR}/Platform/Base/desktop/NativeWindow.cpp
            ${RE_SOURCE_DIR}/Platform/Base/desktop/NativeWindow.h
            )
endif()

add_library(Engine
        ${PRE_HEADER_FILES}
        ${ANIMATION_SOURCE_FILES}
        ${ANIMATION_HEADER_FILES}
        ${AUDIO_SOURCE_FILES}
        ${AUDIO_HEADER_FILES}
        ${BASE_SOURCE_FILES}
        ${BASE_HEADER_FILES}
        ${EFFECT_SOURCE_FILES}
        ${EFFECT_HEADER_FILES}
        ${FILESYSTEM_SOURCE_FILES}
        ${FILESYSTEM_HEADER_FILES}
        ${FONT_SOURCE_FILES}
        ${FONT_HEADER_FILES}
        ${MATH_SOURCE_FILES}
        ${MATH_HEADER_FILES}
        ${MESSAGE_SOURCE_FILES}
        ${MESSAGE_HEADER_FILES}
        ${RENDER_SOURCE_FILES}
        ${RENDER_HEADER_FILES}
        ${RESOURCE_SOURCE_FILES}
        ${RESOURCE_HEADER_FILES}
        ${SCENE_SOURCE_FILES}
        ${SCENE_HEADER_FILES}
        ${UI_SOURCE_FILES}
        ${UI_HEADER_FILES}
        ${UTIL_SOURCE_FILES}
        ${UTIL_HEADER_FILES}
        ${PLATFORM_SOURCE_FILES}
        ${PLATFORM_HEADER_FILES}
        ${PLATFORM_SPECIAL_FILES}
        )
