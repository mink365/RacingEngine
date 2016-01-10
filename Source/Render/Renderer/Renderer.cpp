/*
 * Renderer.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Renderer.h"
#include "RenderTarget.h"

namespace re {

Renderer::Renderer()
    : drawCall(0), trangles(0)
{
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

void Renderer::setDefaultRenderbuffer(uint32_t width, uint32_t height, uint32_t framebuffer,
                                uint32_t colorRenderbuffer, uint32_t depthRenderbuffer, bool depth, bool stencil)
{
    RenderTarget &target = *(this->context.defaultRenderTarget);

    target.framebuffer = framebuffer;
    target.colorRenderBuffer = colorRenderbuffer;
    target.depthRenderbuffer = depthRenderbuffer;
    target.setHasDepthBuffer(depth);
    target.setHasDepthBuffer(stencil);
}

void Renderer::resetRenderTarget()
{
    const RenderTarget &target = *(this->context.defaultRenderTarget);

    this->bindRenderTarget(target);
}

void Renderer::setClearColor(const Color &color)
{
    this->_clearColor = color;
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

void Renderer::readStatistics(size_t *drawCall, size_t *trangles)
{
    if (drawCall != NULL) {
        *drawCall = this->drawCall;
    }

    if (trangles != NULL) {
        *trangles = this->trangles;
    }
}

void Renderer::addDrawCall()
{
    drawCall += 1;
}

void Renderer::addDrawTrangles(size_t count)
{
    trangles += count;
}

void Renderer::resetStatistics()
{
    drawCall = 0;
    trangles = 0;
}

}
