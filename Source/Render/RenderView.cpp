#include "RenderView.h"

#include "RenderTarget.h"
#include "Light/Light.h"

namespace re {

RenderView::RenderView()
{
    this->queueCullFunc = nullptr;
    this->clearFlag = ClearBufferBit::COLOR_BUFFER_BIT
                     | ClearBufferBit::DEPTH_BUFFER_BIT
                     | ClearBufferBit::STENCIL_BUFFER_BIT;

    this->_clearColor = Color::Black;
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

    this->viewMatrix = camera->viewMatrix;
    this->projMatrix = camera->projectionMatrix;

    if (camera->getRenderTarget()) {
        this->renderTarget = camera->getRenderTarget();
        auto size = this->renderTarget->getSize();
        this->viewport.set(0, 0, size.width, size.height);
    } else {
        this->viewport = camera->viewport;
    }

    this->clearFlag = camera->clearFlag;
    this->_clearColor = camera->getClearColor();
    this->queueCullFunc = camera->queueCullFunc;
}

void RenderView::init(LightPtr &light)
{
    // we must set clearColor to WHITE if we want to render the depth to FBO
    this->_clearColor = Color::White;

    if (light->getType() == LightType::Directional) {
        auto& directionalData = light->getDirectionalLightData();

        this->mode = CameraProjectionMode::Orthographic;

        this->setupShadowInfo(light->shadow);

        this->orthoWidth = directionalData.orthoRect.getWidth();

        // TODO: center shold be calc by light direct and curr pos
        Vec3 center{0, 0, 0};
        Vec3 up(0, 1, 0);
        Vec3 p = light->getComponent<Transform>()->getWorldMatrix().getTranslation();
        p.z *= -1;

        this->viewMatrix.lookAt(p, center, up);

        Size ortho(directionalData.orthoRect.getWidth(), directionalData.orthoRect.getHeight());
        this->projMatrix.setOrthoFrustum(-ortho.width / 2.0, ortho.width / 2.0,
                                               -ortho.height / 2.0, ortho.height / 2.0,
                                               zNear, zFar);

    } else if (light->getType() == LightType::Spot) {
        auto& spotData = light->getSpotLightData();

        this->mode = CameraProjectionMode::Perspective;

        this->setupShadowInfo(light->shadow);

        this->fov = spotData.spotAngle;

        // TODO: center shold be calc by light direct and curr pos
        Vec3 center{0, 0, 0};
        Vec3 up(0, 1, 0);

        this->viewMatrix.lookAt(light->getComponent<Transform>()->getWorldMatrix().getTranslation(), center, up);

        this->projMatrix.setPerspective(fov, zNear, zFar);

    } else if (light->getType() == LightType::Point) {
        assert(false);
    } else {
        assert(false);
    }
}

void RenderView::setupShadowInfo(const ShadowInfo &shadowInfo)
{
    this->zNear = shadowInfo.shadowCameraNear;
    this->zFar = shadowInfo.shadowCameraFar;

    if (shadowInfo.renderTarget != nullptr) {
        this->renderTarget = shadowInfo.renderTarget;

        auto size = this->renderTarget->getSize();
        this->viewport.set(0, 0, size.width, size.height);
    }
}

} // namespace re
