TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

LIBS += -lpng -ljpeg
LIBS += -lGL -lGLEW
LIBS += -lglut -lGLU

# 控制defined
DEFINES += _LINUX

SOURCES += \
    tools/ModelLoader/ReadCommon.cpp \
    math/Vector.cpp \
    math/UV.cpp \
    math/Quaternion.cpp \
    math/Matrix.cpp \
    math/Color.cpp \
    tools/ModelLoader/fbx/FbxParser.cpp \
    render/Renderer.cpp \
    render/RenderContext.cpp \
    scene/SceneNode.cpp \
    scene/Node.cpp \
    render/RenderState.cpp \
    texture/Texture.cpp \
    texture/TextureManager.cpp \
    shader/Shader.cpp \
    shader/ShaderManager.cpp \
    shader/Attribute.cpp \
    shader/Uniform.cpp \
    shader/Sampler.cpp \
    shader/ShaderUtil.cpp \
    texture/TextureUtil.cpp \
    image/Image.cpp \
#    examples/image_test.cpp \
#    examples/fbx_test.cpp \
    geometry/Vertex.cpp \
    geometry/Geometry.cpp \
    geometry/Face.cpp \
#    examples/shader_test.cpp \
    render/bufferobject/VertexBuffer.cpp \
    render/bufferobject/IndexBuffer.cpp \
    render/state/BlendState.cpp \
    render/state/DepthState.cpp \
    render/state/SamplerState.cpp \
    render/bufferobject/BufferObjectUtil.cpp \
#    examples/glut_test.cpp
    examples/fbx_render_test.cpp \
    material/Material.cpp \
    material/TextureUnitState.cpp \
    scene/NodeAttribute.cpp \
    scene/Mesh.cpp \
    render/RenderQueue.cpp \
    camera/Camera.cpp \
    light/Light.cpp \
    scene/SceneManager.cpp \
    render/RenderManager.cpp \
    util/StringUtil.cpp \
    animation/Animation.cpp \
    animation/BoneNode.cpp \
    animation/Skeleton.cpp

HEADERS += \
    tools/ModelLoader/ReadCommon.h \
    math/Vector.h \
    math/UV.h \
    math/Quaternion.h \
    math/Matrix.h \
    math/Color.h \
    tools/ModelLoader/fbx/FbxParser.h \
    render/Renderer.h \
    render/RenderContext.h \
    scene/SceneNode.h \
    scene/Node.h \
    render/RenderState.h \
    texture/Texture.h \
    texture/TextureManager.h \
    shader/Shader.h \
    shader/ShaderManager.h \
    shader/Attribute.h \
    shader/Uniform.h \
    shader/Sampler.h \
    shader/ShaderUtil.h \
    texture/TextureUtil.h \
    image/Image.h \
    opengl.h \
    platform.h \
    geometry/Vertex.h \
    geometry/Geometry.h \
    geometry/Face.h \
    render/bufferobject/VertexBuffer.h \
    render/bufferobject/IndexBuffer.h \
    render/state/BlendState.h \
    render/state/DepthState.h \
    render/state/SamplerState.h \
    render/bufferobject/BufferObjectUtil.h \
    math/MathTool.h \
    material/Material.h \
    material/TextureUnitState.h \
    scene/NodeAttribute.h \
    scene/Mesh.h \
    render/RenderQueue.h \
    camera/Camera.h \
    light/Light.h \
    scene/SceneManager.h \
    render/RenderManager.h \
    util/StringUtil.h \
    examples/fbx_render_test.h \
    LogUtil.h \
    animation/Animation.h \
    animation/BoneNode.h \
    animation/Skeleton.h
