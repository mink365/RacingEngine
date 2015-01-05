/*
 * RenderSystem.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "RenderManager.h"
#include "Shader/ShaderUtil.h"
#include "Scene/Component.h"
#include "Light/Light.h"
#include "Light/DirectionalLight.h"
#include "Light/SpotLight.h"
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

void RenderManager::addCamera(CameraPtr &camera)
{
    this->cameraList.push_back(camera);

    this->renderViewDirty = true;
}

void RenderManager::addLight(LightPtr &light)
{
    this->lightList.push_back(light);

    this->renderViewDirty = true;
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

void RenderManager::renderAttribute(const ComponentPtr &attribute)
{
    switch(attribute->getType()) {
        case ComponentType::Mesh:
    {
        MeshPtr mesh = std::dynamic_pointer_cast<Mesh>(attribute);

        this->renderMesh(mesh);
    }
        break;

        case ComponentType::Camera:
        break;

        case ComponentType::Light:
        break;
    }
}

void RenderManager::applyMaterial(Material &material)
{
    // shader
    Shader* shader = nullptr;
    if (this->currRenderView->forceShader) {
        shader = this->currRenderView->forceShader.get();
    } else {
        shader = material.getShader().get();
    }

    shader->getUniform("modelMatrix")->setData((float*)renderer->getModelMatrix());
    shader->getUniform("viewMatrix")->setData((float*)renderer->getViewMatrix());
    shader->getUniform("projectionMatrix")->setData((float*)renderer->getProjectionMatrix());

    ShaderUtil::getInstance().bindShader(shader);

    // TODO: mult pass
    Pass::ptr pass = material.getPass(0);
    int size = pass->getTextureUnitCount();
    for (int i = 0; i < size; ++i) {
        TextureUnitState::ptr state = pass->getTextureUnit(i);

        this->renderer->bindTexture(i, true, *(state->getTexture().get()));

        // TODO: set the matrix of textue in shader ?
    //    this->renderer->setTextureMatrix(0, Mat4().identity());
    }

    this->renderer->applyRenderState(material.getRenderState());
}

void RenderManager::initDefaultRenderState()
{
    this->renderer->resetToRenderState(defaultRenderState);
}

void RenderManager::markRenderViewDirty()
{
    this->renderViewDirty = true;
}

void RenderManager::createRenderViews()
{
    this->renderViewList.clear();

    for (auto light : this->lightList) {
        if (light->getCastShadow()) {
            auto renderView = RenderView::create();

            renderView->init(light);

            // set shader to shadowMap shader
            renderView->forceShader = ShaderManager::getInstance().getShader("depth_rgba");

            if (renderView->renderTarget == nullptr) {
                auto renderTarget = std::make_shared<RenderTarget>();
                renderTarget->setHasDepthBuffer(true);
                renderTarget->setHasStencilBuffer(false);

                auto size = renderTarget->getSize();
                renderView->viewport.set(0, 0, size.width, size.height);

                this->renderer->setupRenderTarget(*renderTarget);

                renderView->renderTarget = renderTarget;
                if (light->getType() == LightType::Spot) {
                    std::static_pointer_cast<SpotLight>(light)->shadow.renderTarget = renderTarget;
                } else if (light->getType() == LightType::Directional) {
                    std::static_pointer_cast<DirectionalLight>(light)->shadow.renderTarget = renderTarget;
                }
            }

            renderView->queueCullFunc = [](int queue) {
                    if (queue == RENDER_QUEUE_UI
                            || queue == 51) {
                        return false;
                    }
                    return true;
                };

            this->renderViewList.push_back(renderView);
        }
    }

    for (auto camera : this->cameraList) {
        auto renderView = RenderView::create();

        renderView->init(camera);

        this->renderViewList.push_back(renderView);
    }

    this->renderViewDirty = false;
}

void RenderManager::renderMesh(const MeshPtr& mesh)
{
    NodePtr node = mesh->getNode();

    this->renderer->setModelMatrix(node->getTransform()->getWorldMatrix());

    Material::ptr material = mesh->getMaterial();

//    Mat4 textureMatrix;
//    auto unit = material->getPass(0)->getTextureUnit(0);
//    textureMatrix.setScaling(unit->getScale().x, unit->getScale().y, 0);

//    shader->getUniform("textureMatrix")->setData((float*)textureMatrix);

    this->renderer->bindBuffer(*(mesh));

    this->applyMaterial(*(material.get()));

//    LOG_E("Render mesh: %s, %d", mesh->getName().c_str(), mesh->getMaterial()->getQueueID());

    this->renderer->renderMesh(*(mesh));
}

void RenderManager::render()
{
    this->renderer->resetStatistics();
    this->initDefaultRenderState();

    this->renderQueue.sortRenderableList();

    std::vector<RenderableList *> &lists = this->renderQueue.lists;

//    if (this->renderViewDirty) {
        this->createRenderViews();
//    }

//    LOG_E("start Render ------------------------------------------------");

    for (auto renderView : this->renderViewList) {
        this->activeRenderView(renderView);

        auto func = renderView->queueCullFunc;

        for (auto renderableList : lists) {
            int queueID = renderableList->listType;

            if (!func || func(queueID)) {
                this->renderList(*renderableList);
            }
        }
    }

//    LOG_E("end Render ------------------------------------------------");
}

void RenderManager::activeRenderView(RenderView::ptr& view)
{
    if (view == this->renderViewList[0]) {
        auto oldView = this->renderViewList[1];

        this->renderer->setProjectionMatrix(view->projMatrix);
        this->renderer->setViewMatrix(view->viewMatrix);
    } else {
        this->renderer->setProjectionMatrix(view->projMatrix);
        this->renderer->setViewMatrix(view->viewMatrix);
    }

    // bind RenderTarget and viewport
    this->renderer->setViewport(view->viewport);

    if (view->renderTarget != nullptr) {
        this->renderer->bindRenderTarget(*(view->renderTarget));
    } else {
        this->renderer->resetRenderTarget();
    }

    if (view->clearFlag) {
        this->renderer->setClearColor(view->_clearColor);
        this->renderer->cleanBuffers(view->clearFlag);
    }

    this->currRenderView = view;
}

void RenderManager::clear()
{
    this->renderQueue.clear();
}

}
