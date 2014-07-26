/*
 * RenderSystem.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "RenderManager.h"
#include "Shader/ShaderUtil.h"
#include "Scene/NodeAttribute.h"
#include "Scene/Light/Light.h"
#include "Renderer/Renderer.h"
#include "RenderTarget.h"
#include "Shader/ShaderManager.h"

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

void RenderManager::setRenderer(std::shared_ptr<Renderer>& renderer)
{
    this->renderer = renderer;
}

Renderer &RenderManager::getRenderer()
{
    return *renderer;
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
    ShaderUtil::getInstance().bindShader(material.getShader().get());

    // TODO: mult pass
    Pass::ptr pass = material.getPass(0);
    int size = pass->getTextureUnitCount();
    for (int i = 0; i < size; ++i) {
        TextureUnitState::ptr state = pass->getTextureUnit(i);

        this->renderer->setTexture(i, true, *(state->getActivityTexture().get()));

        // TODO: set the matrix of textue in shader ?
    //    this->renderer->setTextureMatrix(0, Mat4().identity());
    }

    this->renderer->applyRenderState(material.getRenderState());
}

void RenderManager::initDefaultRenderState()
{
    this->renderer->resetToRenderState(defaultRenderState);
}

void RenderManager::createRenderViews()
{
    this->renderViewList.clear();

    for (auto light : this->lightList) {
        if (light->getCastShadow()) {
            auto renderView = RenderView::create();

            renderView->init(light);

            // set shader to shadowMap shader
            renderView->forceShader = ShaderManager::getInstance().getShader("ShadowMap");

            auto renderTarget = std::make_shared<RenderTarget>();
            renderTarget->setHasDepthBuffer(true);
            renderTarget->setHasStencilBuffer(false);

            auto size = renderTarget->getSize();
            renderView->viewport.set(-size.width / 2.0, -size.height/2.0, size.width, size.height);

            this->renderer->setupRenderTarget(*renderTarget);
            renderView->renderTarget = renderTarget;

            this->renderViewList.push_back(renderView);
        }
    }

    for (auto camera : this->cameraList) {
        auto renderView = RenderView::create();

        renderView->init(camera);

        this->renderViewList.push_back(renderView);
    }
}

void RenderManager::renderMesh(const MeshPtr& mesh)
{
    SceneNodePtr node = mesh->getNode();

    this->renderer->setWorldMatrix(node->getWorldMatrix());

    Material::ptr material = mesh->getMaterial();
    Geometry::ptr geometry = mesh->getGeometry();

    std::shared_ptr<Shader> shader = material->getShader();
    shader->getUniform("modelMatrix")->setData((float*)node->getWorldMatrix());
    shader->getUniform("viewMatrix")->setData((float*)currRenderView->viewMatrix);
    shader->getUniform("projectionMatrix")->setData((float*)currRenderView->projMatrix);

    Mat4 textureMatrix;
    auto unit = material->getPass(0)->getTextureUnit(0);
    textureMatrix.setScaling(unit->getScale().x, unit->getScale().y, 0);

    shader->getUniform("textureMatrix")->setData((float*)textureMatrix);

    this->renderer->bindShader(*(material->getShader()));

    this->renderer->bindBuffer(*(geometry));

    this->applyMaterial(*(material.get()));

    this->renderer->renderMesh(*(geometry));
}

void RenderManager::render()
{
    this->renderQueue.sortRenderableList();

    std::vector<RenderableList *> &lists = this->renderQueue.lists;

    this->createRenderViews();

    for (auto renderView : this->renderViewList) {
        this->activeRenderView(renderView);

        auto& func = renderView->queueCullFunc;

        for (auto renderableList : lists) {
            int queueID = renderableList->listType;

            if (func(queueID)) {
                this->renderList(*renderableList);
            }
        }
    }
}

void RenderManager::activeRenderView(RenderView::ptr& view)
{
    this->renderer->setProjectionMatrix(view->projMatrix);
    this->renderer->setViewMatrix(view->viewMatrix);

    if (view->clearFlag) {
        this->renderer->cleanBuffers(view->clearFlag);
    }

    // bind RenderTarget and viewport
    this->renderer->setViewport(view->viewport);

    if (view->renderTarget != nullptr) {
        this->renderer->bindRenderTarget(*(view->renderTarget));
    } else {
        this->renderer->resetRenderTarget();
    }

    this->currRenderView = view;
}

void RenderManager::clear()
{
    this->renderQueue.clear();
}

}
