/*
 * RenderSystem.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "RenderManager.h"
#include "Shader/ShaderUtil.h"
#include "Scene/NodeAttribute.h"

namespace re {

const RenderState RenderManager::defaultRenderState = RenderState();

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

void RenderManager::addCamera(CameraPtr camera)
{
    this->cameraList.push_back(camera);
}

RenderQueue &RenderManager::getRenderQueue()
{
    return renderQueue;
}

Renderer &RenderManager::getRenderer()
{
    return renderer;
}

void RenderManager::renderList(const RenderableList &list)
{
    for (auto att : list.attributes) {
        this->renderAttribute(att);
    }
}

void RenderManager::renderAttribute(const NodeAttributePtr &attribute)
{
    switch(attribute->getType()) {
        case NodeAttributeType::Mesh:
    {
        //TODO:
        MeshPtr mesh = std::dynamic_pointer_cast<Mesh>(attribute);

        this->renderMesh(mesh);
    }
        break;

        case NodeAttributeType::Camera:
        break;

        case NodeAttributeType::Light:
        break;
    }
}

void RenderManager::applyMaterial(Material &material)
{
    // shader
    ShaderUtil::getInstance().bindShader(material.getShder().get());

    // TODO: mult pass
    Pass::ptr pass = material.getPass(0);
    int size = pass->getTextureUnitCount();
    for (int i = 0; i < size; ++i) {
        TextureUnitState::ptr state = pass->getTextureUnit(i);

        this->renderer.setTexture(i, true, *(state->getActivityTexture().get()));

        // TODO: set the matrix of textue in shader ?
    //    this->renderer.setTextureMatrix(0, Mat4().identity());
    }

    this->renderer.applyRenderState(material.getRenderState());
}

void RenderManager::initDefaultRenderState()
{
    this->renderer.resetToRenderState(defaultRenderState);
}

void RenderManager::renderMesh(const MeshPtr& mesh)
{
    SceneNodePtr node = mesh->getNode();

    this->renderer.setWorldMatrix(node->getWorldMatrix());

    Material::ptr material = mesh->getMaterial();
    Geometry::ptr geometry = mesh->getGeometry();

    std::shared_ptr<Shader> shader = material->getShder();
    shader->getUniform("model")->setData((float*)node->getWorldMatrix());
    shader->getUniform("view")->setData((float*)currCamera->getViewMatrix());
    shader->getUniform("projection")->setData((float*)currCamera->getProjectionMatrix());

    glUseProgram(material->getShder()->getProgram());

    this->renderer.bindBuffer(*(geometry.get()));

    this->applyMaterial(*(material.get()));

    this->renderer.renderMesh(*(geometry.get()));
}

void RenderManager::render()
{
    this->setDefaultRenderEnv();

    this->renderQueue.sortRenderableList();

    std::vector<RenderableList *> &lists = this->renderQueue.lists;

    for (auto camera : this->cameraList) {
        this->activeCamera(camera);

        auto func = camera->getQueueCullFunc();

        for (auto renderableList : lists) {
            int queueID = renderableList->listType;

            if (func(queueID)) {
                this->renderList(*renderableList);
            }
        }
    }
}

void RenderManager::setDefaultRenderEnv()
{
    // apply default
    this->renderer.applyRenderState(defaultRenderState);
}

void RenderManager::activeCamera(CameraPtr camera)
{
    this->renderer.setProjectionMatrix(camera->getProjectionMatrix());
    this->renderer.setViewMatrix(camera->getViewMatrix());

    if (camera->getClearFlag()) {
        this->renderer.cleanBuffers(camera->getClearFlag());
    }

    this->currCamera = camera;
}

void RenderManager::clear()
{
    this->renderQueue.clear();
}

}
