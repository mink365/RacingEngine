#include "RenderView.h"

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

        this->zNear = dirLight->shadow.shadowCameraNear;
        this->zFar = dirLight->shadow.shadowCameraFar;

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

        this->zNear = spotLight->shadow.shadowCameraNear;
        this->zFar = spotLight->shadow.shadowCameraFar;

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

} // namespace re
