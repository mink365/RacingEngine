#include "GLES2Renderer.h"

#include "opengl.h"

#include "Shader/Shader.h"
#include "RenderTarget.h"
#include "Texture/Texture.h"
#include "RenderState.h"
#include "Geometry/Geometry.h"
#include "Render/Mesh.h"
#include "Texture/TextureUtil.h"

namespace re {

GLuint CreateFrameBuffer() {
    GLuint id;

    glGenFramebuffers(1, &id);

    return id;
}

GLuint CreateRenderBuffer() {
    GLuint id;

    glGenRenderbuffers(1, &id);

    return id;
}

void SetupFrameBuffer(GLuint buffer, RenderTarget& target, GLenum textureTarget) {
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureTarget, target.getTexture()->getGlID(), 0);
}

void SetupRenderBuffer(GLuint buffer, RenderTarget& target) {
    glBindRenderbuffer(GL_RENDERBUFFER, buffer);

    if (target.getHasDepthBuffer() && ! target.getHasStencilBuffer()) {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,
                              target.getTexture()->getWidth(),
                              target.getTexture()->getHeight());

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer);
    } else if (target.getHasDepthBuffer() && target.getHasStencilBuffer()) {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL,
                              target.getTexture()->getWidth(),
                              target.getTexture()->getHeight());

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer);
    } else {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4,
                              target.getTexture()->getWidth(),
                              target.getTexture()->getHeight());
    }
}

extern const GLenum TextureTargetTypeToGL(Texture::TargetType type);

GLES2Renderer::GLES2Renderer()
{
}

void GLES2Renderer::setViewport(const Rect &viewport)
{
    if (this->context.viewport != viewport) {

        glViewport(viewport.getMinX(), viewport.getMinY(),
                   viewport.getWidth(), viewport.getHeight());

        this->context.viewport = viewport;
    }
}

void GLES2Renderer::bindTexture(int unit, bool enable, const Texture& texture)
{
    GLenum target = TextureTargetTypeToGL(texture.getTarget());

    if (this->context.textureUnits[unit].textureId != texture.getGlID()) {
        this->activateTextureUnit(unit);

        glBindTexture(target, texture.getGlID());

        this->context.textureUnits[unit].textureId = texture.getGlID();
    }
}

void GLES2Renderer::bindRenderTarget(const RenderTarget &target)
{
    GPU_ID framebuffer = 0;
    if (target.getType() == RenderTargetType::CUBE) {
        const RenderTargetCube& cubeTarget = *(dynamic_cast<const RenderTargetCube*>(&target));

        framebuffer = cubeTarget.framebuffers[cubeTarget.activeCubeFace];
    } else {
        framebuffer = target.framebuffer;
    }

    if (framebuffer != context.boundFBO) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        context.boundFBO = framebuffer;
    }
}

void GLES2Renderer::bindShader(const Shader &shader)
{
    glUseProgram(shader.getProgram());
}

void GLES2Renderer::activateTextureUnit(int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);

    this->context.textureUnits[unit].unitEnabled = true;
}

void GLES2Renderer::setupRenderTarget(RenderTarget &target)
{
    auto texture = Create<Texture>(target.getSize().width, target.getSize().height, 0);
    target.setTexture(texture);
    // TODO: param base on POT
    texture->setFormat(TextureFormat::RGBA8);

    bool isTargetPowerOfTwo = IsPowerOfTwo(texture->getWidth()) && IsPowerOfTwo(texture->getHeight());

    if (target.getType() == RenderTargetType::CUBE) {
        texture->setTarget(Texture::TargetType::TEXTURE_CUBE_MAP);

        RenderTargetCube& cubeTarget = *(dynamic_cast<RenderTargetCube*>(&target));

        for (int i = 0; i < 6; ++i) {
            cubeTarget.framebuffers[i] = CreateFrameBuffer();
            cubeTarget.renderbuffers[i] = CreateRenderBuffer();

            TextureUtil::UploadTextureToHardware(nullptr, *texture, i);

            SetupFrameBuffer(cubeTarget.framebuffers[i], target, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
            SetupRenderBuffer(cubeTarget.renderbuffers[i], target);
        }

        if ( isTargetPowerOfTwo && target.getGenerateMipmaps() )
            glGenerateMipmap( GL_TEXTURE_2D );
    } else {
        target.framebuffer = CreateFrameBuffer();

        if (target.shareDepthFrom == nullptr) {
            target.depthRenderbuffer = CreateRenderBuffer();
        } else {
            target.depthRenderbuffer = target.shareDepthFrom->depthRenderbuffer;
        }

        texture->setTarget(Texture::TargetType::TEXTURE_2D);
        TextureUtil::UploadTextureToHardware(nullptr, *texture);

        SetupFrameBuffer(target.framebuffer, target, GL_TEXTURE_2D);

        if (target.shareDepthFrom == nullptr) {
            SetupRenderBuffer(target.depthRenderbuffer, target);
        } else {
            // no need to allocate memory, just bind

            if (target.getHasDepthBuffer() && ! target.getHasStencilBuffer()) {
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, target.depthRenderbuffer);
            } else if (target.getHasDepthBuffer() && target.getHasStencilBuffer()) {
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, target.depthRenderbuffer);
            }
        }

        if ( isTargetPowerOfTwo && target.getGenerateMipmaps() )
            glGenerateMipmap( GL_TEXTURE_2D );
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLES2Renderer::bindBuffer(const MeshData &meshData)
{
    for (auto& stream : meshData.vertexStreams) {
        glBindBuffer(GL_ARRAY_BUFFER, stream.stream.vbo);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData.indexStream.vboIB);
}

void GLES2Renderer::renderMesh(const MeshData &meshData)
{
    GLenum mode = GL_TRIANGLES;
    switch(context.renderState.polygonMode)
    {
    case PolygonMode::Fill:
        mode = GL_TRIANGLES;
        break;
    case PolygonMode::Line:
        mode = GL_LINES;
        break;
    case PolygonMode::Point:
        mode = GL_POINTS;
        break;
    }

    glDrawElements(mode, meshData.indexStream.nIndices, GL_UNSIGNED_SHORT, NULL);

    this->addDrawCall();
    this->addDrawTrangles(meshData.indexStream.nIndices / 3);
}

void GLES2Renderer::setClearColor(const Color &color)
{
    Renderer::setClearColor(color);

    glClearColor(color.r, color.g, color.b, color.a);
}

void GLES2Renderer::cleanBuffers(bool color, bool depth, bool stencil)
{
    int bits = 0;
    if (color) {
        bits |= GL_COLOR_BUFFER_BIT;
    }

    if (depth) {
        bits |= GL_DEPTH_BUFFER_BIT;
    }

    if (stencil) {
        bits |= GL_STENCIL_BUFFER_BIT;
    }

    if (bits != 0) {
        glClear(bits);
    }
}

GLenum GetTestFunc(TestFunction func) {
    switch(func) {
    case TestFunction::Always:
        return GL_ALWAYS;
    case TestFunction::Equal:
        return GL_EQUAL;
    case TestFunction::Greater:
        return GL_GREATER;
    case TestFunction::GreaterOrEqual:
        return GL_GEQUAL;
    case TestFunction::Less:
        return GL_LESS;
    case TestFunction::LessOrEqual:
        return GL_LEQUAL;
    case TestFunction::Never:
        return GL_NEVER;
    case TestFunction::NotEqual:
        return GL_NOTEQUAL;
    }

    return GL_ALWAYS;
}

void GetBlendMode(BlendMode mode, GLenum* result) {
    switch(mode) {
    case BlendMode::Additive:
        result[0] = GL_ONE;
        result[1] = GL_ONE;
        break;
    case BlendMode::Alpha:
        result[0] = GL_SRC_ALPHA;
        result[1] = GL_ONE_MINUS_SRC_ALPHA;
        break;
    case BlendMode::AlphaAdditive:
        result[0] = GL_SRC_ALPHA;
        result[1] = GL_ONE;
        break;
    case BlendMode::Color:
        result[0] = GL_ONE;
        result[1] = GL_ONE_MINUS_SRC_COLOR;
        break;
    case BlendMode::Modulate:
        result[0] = GL_DST_COLOR;
        result[1] = GL_ZERO;
        break;
    case BlendMode::ModulateX2:
        result[0] = GL_DST_COLOR;
        result[1] = GL_SRC_COLOR;
        break;
    case BlendMode::PremultAlpha:
        result[0] = GL_ONE;
        result[1] = GL_ONE_MINUS_SRC_ALPHA;
        break;
    }
}

GLenum GetFaceCullMode(FaceCullMode mode) {
    switch(mode) {
    case FaceCullMode::Back:
        return GL_BACK;
    case FaceCullMode::Front:
        return GL_FRONT;
    case FaceCullMode::FrontAndBack:
        return GL_FRONT_AND_BACK;
    default:
        break;
    }

    return GL_FRONT;
}

#if !GLES
GLenum GetPolygonMode(PolygonMode mode) {
    switch(mode) {
        case PolygonMode::Fill:
            return GL_FILL;
        case PolygonMode::Line:
            return GL_LINE;
        case PolygonMode::Point:
            return GL_POINT;
    }
}
#endif

GLenum GetStencilOperation(StencilOperation op)
{
    switch(op)
    {
    case StencilOperation::Keep:
        return GL_KEEP;
    case StencilOperation::Zero:
        return GL_ZERO;
    case StencilOperation::Replace:
        return GL_REPLACE;
    case StencilOperation::Increment:
        return GL_INCR;
    case StencilOperation::IncrementWrap:
        return GL_INCR_WRAP;
    case StencilOperation::Decrement:
        return GL_DECR;
    case StencilOperation::DecrementWrap:
        return GL_DECR_WRAP;
    case StencilOperation::Invert:
        return GL_INVERT;
    }
}

void GLES2Renderer::applyRenderState(const RenderState &state, bool force)
{
    if (force || this->context.renderState.depthWrite != state.depthWrite) {
        glDepthMask(state.depthWrite);

        context.renderState.depthWrite = state.depthWrite;
    }

    if (force || this->context.renderState.depthState != state.depthState) {
        if (state.depthState.depthTestEnable) {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GetTestFunc(state.depthState.function));
        } else {
            glDisable(GL_DEPTH_TEST);
        }

        context.renderState.depthState = state.depthState;
    }


    if (force || context.renderState.alphaState != state.alphaState) {
        context.renderState.alphaState = state.alphaState;
    }

    if (force || context.renderState.blendState != state.blendState) {
        if (state.blendState.blendEnable) {
            glEnable(GL_BLEND);

            GLenum rgbFunc[2], alphaFunc[2];
            GetBlendMode(state.blendState.blendModeRGB, rgbFunc);
            GetBlendMode(state.blendState.blendModeAlpha, alphaFunc);

            glBlendFuncSeparate(rgbFunc[0], rgbFunc[1], alphaFunc[0], alphaFunc[1]);
        } else {
            glDisable(GL_BLEND);
        }

        context.renderState.blendState = state.blendState;
    }

    if (force || context.renderState.faceCullMode != state.faceCullMode) {
        if (state.faceCullMode != FaceCullMode::Off) {
            glEnable(GL_CULL_FACE);

            // glFrontFace(GL_CCW);
            glCullFace(GetFaceCullMode(state.faceCullMode));
        } else {
            glDisable(GL_CULL_FACE);
        }

        context.renderState.faceCullMode = state.faceCullMode;
    }

    if (force || context.renderState.polygonMode != state.polygonMode) {

//        GLenum mode = GetPolygonMode(state.polygonMode);
//        glPolygonMode(GL_FRONT_AND_BACK, mode);
        glLineWidth(3.0f);

        context.renderState.polygonMode = state.polygonMode;
    }

    if (force || context.renderState.stencilState != state.stencilState) {
        const StencilState& stencilState = state.stencilState;

        if (stencilState.testEnable) {
            glEnable(GL_STENCIL_TEST);

            glStencilFunc(GetTestFunc(stencilState.function), stencilState.refValue, 0xFF);
            glStencilOp(GetStencilOperation(stencilState.failOperation),
                        GetStencilOperation(stencilState.depthFailOperation),
                        GetStencilOperation(stencilState.depthPassOperation));

            glStencilMask(stencilState.maskValue);
        } else {
            glDisable(GL_STENCIL_TEST);
        }

        context.renderState.stencilState = state.stencilState;
    }
}

} // namespace re
