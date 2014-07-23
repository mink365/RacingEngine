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

void Renderer::resetToRenderState(const RenderState &state)
{
    this->applyRenderState(state, true);
}

}
