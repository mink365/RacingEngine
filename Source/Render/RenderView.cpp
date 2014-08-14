#include "RenderView.h"

#include "RenderTarget.h"
#include "Light/Light.h"
#include "Light/DirectionalLight.h"
#include "Light/SpotLight.h"

namespace re {

RenderView::RenderView()
{
    this->queueCullFunc = nullptr;
    this->clearFlag = ClearBufferBit::COLOR_BUFFER_BIT
                     | ClearBufferBit::DEPTH_BUFFER_BIT
                     | ClearBufferBit::STENCIL_BUFFER_BIT;

    this->_clearColor = Color::BLACK;
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
    // we must set clearColor to WHITE if we want to render the depth to FBO
    this->_clearColor = Color::WHITE;

    if (light->getType() == LightType::Directional) {
        auto dirLight = std::dynamic_pointer_cast<DirectionalLight>(light);

        this->mode = CameraProjectionMode::Orthographic;

        this->setupShadowInfo(dirLight->shadow);

        this->orthoWidth = dirLight->orthoRect.getWidth();

        // TODO: center shold be calc by light direct and curr pos
        Vec3 center{0, 0, 0};
        Vec3 up(0, 1, 0);
        Vec3 p = light->worldMatrix.getTranslation();
        p.z *= -1;

        this->viewMatrix.lookAt(p, center, up);

        Size ortho(dirLight->orthoRect.getWidth(), dirLight->orthoRect.getHeight());
        this->projMatrix.setOrthoFrustum(-ortho.width / 2.0, ortho.width / 2.0,
                                               -ortho.height / 2.0, ortho.height / 2.0,
                                               zNear, zFar);

    } else if (light->getType() == LightType::Spot) {
        auto spotLight = std::dynamic_pointer_cast<SpotLight>(light);

        this->mode = CameraProjectionMode::Perspective;

        this->setupShadowInfo(spotLight->shadow);

        this->fov = spotLight->spotAngle;

        // TODO: center shold be calc by light direct and curr pos
        Vec3 center{0, 0, 0};
        Vec3 up(0, 1, 0);

        this->viewMatrix.lookAt(light->worldMatrix.getTranslation(), center, up);

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
