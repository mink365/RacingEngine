/*
 * Renderer.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Renderer.h"

namespace re {

Renderer::Renderer() {
	// TODO Auto-generated constructor stub

}

Renderer::~Renderer() {
    // TODO Auto-generated destructor stub
}

void Renderer::setViewPort(int x, int y, int width, int height)
{
    this->viewport.set(x, y, width, height);
}

void Renderer::setWorldMatrix(const Mat4 &mat)
{

}

void Renderer::setViewMatrix(const Mat4 &mat)
{
    this->viewMatrix = mat;
}

void Renderer::setProjectionMatrix(const Mat4 &mat)
{
    this->projMatrix = mat;
}

void Renderer::setTexture(int unit, bool enable, const Texture& texture)
{
    // TODO: size assert

    if (this->context.textureUnits[unit].textureId != texture.getGlID()) {
        glBindTexture(GL_TEXTURE_2D, texture.getGlID());

        this->context.textureUnits[unit].textureId = texture.getGlID();
    }
}

void Renderer::activateTextureUnit(int unit)
{
    // TODO: assert the texture unit size in hardware

    glActiveTexture(GL_TEXTURE0 + unit);
    glClientActiveTexture(GL_TEXTURE0 + unit);

    this->context.textureUnits[unit].unitEnabled = true;
}

void Renderer::bindBuffer(const Geometry &geometry)
{
    glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.ibo.vboIB);
}

void Renderer::renderMesh(const Geometry &geometry)
{
    glDrawElements(GL_TRIANGLES, geometry.ibo.nIndices, GL_UNSIGNED_SHORT, NULL);
}

void Renderer::cleanBuffers(bool color, bool depth, bool stencil)
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
        this->cleanBuffers(bits);
    }
}

void Renderer::cleanBuffers(int flag)
{
    glClear(flag);
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
    }
}

void Renderer::applyRenderState(const RenderState &state, bool force)
{
    if (force || this->context.depthState != state.depthState) {
        if (state.depthState.depthTestEnable) {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GetTestFunc(state.depthState.function));
            glDepthMask(state.depthState.depthWrite);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        context.depthState = state.depthState;
    }


    if (force || context.alphaState != state.alphaState) {
        if (state.alphaState.alphaTestEnable) {
            glEnable(GL_ALPHA_TEST);

            glAlphaFunc(GetTestFunc(state.alphaState.function), state.alphaState.alphaFallOff);
        } else {
            glDisable(GL_ALPHA_TEST);
        }

        context.alphaState = state.alphaState;
    }

    if (force || context.blendState != state.blendState) {
        if (state.blendState.blendEnable) {
            glEnable(GL_BLEND);

            GLenum rgbFunc[2], alphaFunc[2];
            GetBlendMode(state.blendState.blendModeRGB, rgbFunc);
            GetBlendMode(state.blendState.blendModeAlpha, alphaFunc);

            glBlendFuncSeparate(rgbFunc[0], rgbFunc[2], alphaFunc[0], alphaFunc[1]);
        } else {
            glDisable(GL_BLEND);
        }

        context.blendState = state.blendState;
    }

    if (force || context.faceCullState != state.faceCullState) {
        if (state.faceCullState.faceCullEnable) {
            glEnable(GL_CULL_FACE);

            // glFrontFace(GL_CCW);
            glCullFace(GetFaceCullMode(state.faceCullState.cullMode));
        } else {
            glDisable(GL_CULL_FACE);
        }

        context.faceCullState = state.faceCullState;
    }

    if (force || context.stencilState != state.stencilState) {
        // TODO:
        if (state.stencilState.stencilTestEnable) {

        } else {

        }

        context.stencilState = state.stencilState;
    }
}

void Renderer::resetToRenderState(const RenderState &state)
{
    this->applyRenderState(state, true);
}

}
