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
}

void Renderer::setWorldMatrix(const Mat4 &mat)
{

}

void Renderer::setViewMatrix(const Mat4 &mat)
{
}

void Renderer::setProjectionMatrix(const Mat4 &mat)
{
}

void Renderer::setTexture(int unit, bool enable, const Texture& texture)
{
    // TODO: unit
    if (this->context.textureID != texture.getGlID()) {
        glBindTexture(GL_TEXTURE_2D, texture.getGlID());

        this->context.textureID = texture.getGlID();
    }
}

void Renderer::setTextureMatrix(int unit, const Mat4 &mat)
{
    // TODO:
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

void Renderer::applyRenderState(const RenderState &state)
{
    this->context.depthTestEnabled = state.getDepthTest();
}

void Renderer::resetToRenderState(const RenderState &state)
{
    this->context.depthTestEnabled = state.getDepthTest();

//    this->context.textureID = state.

    this->context.textureID = 0;
}

}
