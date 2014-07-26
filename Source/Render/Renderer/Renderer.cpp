/*
 * Renderer.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Renderer.h"
#include "Scene/Light/Light.h"
#include "Scene/Light/SpotLight.h"
#include "Scene/Light/DirectionalLight.h"

namespace re {

Renderer::Renderer() {
}

Renderer::~Renderer() {
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

void RenderView::init(CameraPtr &camera)
{
    this->mode = camera->mode;

    this->zNear = camera->zNear;
    this->zFar = camera->zFar;

    if (mode == CameraProjectionMode::Orthographic) {
        this->orthoWidth = camera->orthoWidth;
    } else {
        this->fov = camera->fov;
    }

    this->viewport = camera->viewport;
    this->viewMatrix = camera->viewMatrix;
    this->projMatrix = camera->projectionMatrix;

    this->clearFlag = camera->clearFlag;
    this->queueCullFunc = camera->queueCullFunc;
}

void RenderView::init(LightPtr &light)
{
    if (light->getType() == LightType::Directional) {
        auto dirLight = std::dynamic_pointer_cast<DirectionalLight>(light);

        this->mode = CameraProjectionMode::Orthographic;

        this->zNear = 10;
        this->zFar = 3000;

        this->orthoWidth = dirLight->orthoRect.getWidth();

        // TODO: center shold be calc by light direct and curr pos
        Vec3 center{0, 0, 0};
        Vec3 up(0, 1, 0);

        this->viewMatrix.lookAt(light->worldMatrix.getTranslation(), center, up);

        Size ortho(dirLight->orthoRect.getWidth(), dirLight->orthoRect.getHeight());
        this->projMatrix.setOrthoFrustum(-ortho.width / 2.0, ortho.width / 2.0,
                                               -ortho.height / 2.0, ortho.height / 2.0,
                                               zNear, zFar);


    } else if (light->getType() == LightType::Spot) {
        auto spotLight = std::dynamic_pointer_cast<SpotLight>(light);

        this->mode = CameraProjectionMode::Perspective;

        this->zNear = 10;
        this->zFar = 3000;

        this->fov = spotLight->spotAngle;

        // TODO: center shold be calc by light direct and curr pos
        Vec3 center{0, 0, 0};
        Vec3 up(0, 1, 0);

        this->viewMatrix.lookAt(light->worldMatrix.getTranslation(), center, up);

        this->projMatrix.setPerspective(fov, zNear, zFar);
    } else if (light->getType() == LightType::Point) {

    }
}

}
