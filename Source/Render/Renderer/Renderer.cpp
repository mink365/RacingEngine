/*
 * Renderer.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Renderer.h"

namespace re {

Renderer::Renderer() {
}

Renderer::~Renderer() {
}

void Renderer::setModelMatrix(const Mat4 &mat)
{
    this->modelMatrix = mat;
}

void Renderer::setViewMatrix(const Mat4 &mat)
{
    this->viewMatrix = mat;
}

void Renderer::setProjectionMatrix(const Mat4 &mat)
{
    this->projMatrix = mat;
}

const Mat4 &Renderer::getModelMatrix() const
{
    return this->modelMatrix;
}

const Mat4 &Renderer::getViewMatrix() const
{
    return this->viewMatrix;
}

const Mat4 &Renderer::getProjectionMatrix() const
{
    return this->projMatrix;
}

void Renderer::cleanBuffers(int flag)
{
    bool color = flag & ClearBufferBit::COLOR_BUFFER_BIT;
    bool depth = flag & ClearBufferBit::DEPTH_BUFFER_BIT;
    bool stencil = flag & ClearBufferBit::STENCIL_BUFFER_BIT;

    this->cleanBuffers(color, depth, stencil);
}

void Renderer::resetToRenderState(const RenderState &state)
{
    this->applyRenderState(state, true);
}

}
