/*
 * RenderSystem.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "RenderManager.h"
#include <shader/ShaderUtil.h>

const RenderState RenderManager::defaultRenderState = RenderState();

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

RenderQueue &RenderManager::getRenderQueue()
{
    return renderQueue;
}

Renderer &RenderManager::getRenderer()
{
    return renderer;
}

void RenderManager::renderList(RenderableList &list)
{
    for (auto node : list.nodes) {
        MeshPtr mesh = dynamic_pointer_cast<Mesh>(node);

        this->renderMesh(mesh);
    }
}

void RenderManager::applyMaterial(Material &material)
{
    // shader
    ShaderUtil::getInstance().bindShader(material.getShder());

    TextureUnitState state = material.getTexture();

    this->renderer.setTexture(0, true, state.getActivityTexture());

    // TODO: 纹理矩阵
    this->renderer.setTextureMatrix(0, reMat4().identity());

    this->renderer.applyRenderState(material.getRenderState());
}

void RenderManager::initDefaultRenderState()
{
    this->renderer.resetToRenderState(defaultRenderState);
}

void RenderManager::renderMesh(MeshPtr mesh)
{
    this->renderer.setWorldMatrix(mesh->getWorldMatrix());
    mesh->getMaterial().getShder()->getUniform("model")->setData((float*)mesh->getWorldMatrix());

    glUseProgram(mesh->getMaterial().getShder()->getProgram());

    this->renderer.bindBuffer(mesh->getGeometry());

    this->applyMaterial(mesh->getMaterial());

    this->renderer.renderMesh(mesh->getGeometry());
}

void RenderManager::render()
{
    this->setDefaultRenderEnv();

    this->renderQueue.sortRenderableList();

    std::vector<RenderableList *> &lists = this->renderQueue.lists;
    std::vector<RenderableList *>::iterator iter;
    for (iter = lists.begin(); iter != lists.end(); ++iter) {
        RenderableList *list = *iter;

        this->renderList(*list);
    }
}

void RenderManager::setDefaultRenderEnv()
{
    this->renderer.setProjectionMatrix(this->camera.getProjectionMatrix());
    this->renderer.setViewMatrix(this->camera.getViewMatrix());

    // apply default
    this->renderer.applyRenderState(defaultRenderState);

    // clear buffer or not
}

void RenderManager::clear()
{
    this->renderQueue.clear();
}
