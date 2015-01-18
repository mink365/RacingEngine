#include "Camera.h"
#include "Scene/Node.h"

namespace re {

Camera::Camera()
{
    this->type = ComponentType::Camera;

    this->queueCullFunc = [](int){return false;};
    this->clearFlag = ClearBufferBit::COLOR_BUFFER_BIT
                     | ClearBufferBit::DEPTH_BUFFER_BIT
                     | ClearBufferBit::STENCIL_BUFFER_BIT;

    this->clearColor = Color::Black;

    this->fov = 45;
    this->orthoWidth = 0;
    this->zNear = 0;
    this->zFar = 0;

    this->mode = CameraProjectionMode::Perspective;
}

void Camera::setDepthField(float near, float far)
{
    this->zNear = near;
    this->zFar = far;
}

void Camera::setViewport(float width, float height)
{
    this->viewport.size.set(width, height);
}

void Camera::setViewport(Rect v)
{
    this->viewport = v;
}

const Rect &Camera::getViewport() const
{
    return this->viewport;
}

void Camera::setProjectionMode(CameraProjectionMode mode)
{
    this->mode = mode;
}

CameraProjectionMode Camera::getProjectionMode() const
{
    return this->mode;
}

void Camera::setFieldOfView(float fov)
{
    this->fov = fov;
}

float Camera::getFieldOfView() const
{
    return this->fov;
}

void Camera::setOrthoWidth(float v)
{
    this->orthoWidth = v;
}

Size Camera::getOrthoSize() const
{
    float aspect = this->viewport.getWidth() / this->viewport.getHeight();

    float orthoHeight = orthoWidth / aspect;

    return Size(orthoWidth, orthoHeight);
}

void Camera::setView(const Vec3 &eye, const Vec3 &center, const Vec3 &up)
{
    this->center = center;

    auto transform = this->getComponent<Transform>();
    auto parent = this->getComponent<Transform>();

    Vec3 position;
    if (parent) {
        // use the world position of the node
        position = parent->getWorldMatrix().inverse() * eye;
    } else {
        position = eye;
    }
    transform->setLocalTranslation(position);

    //       dir  up
    //         ^  ^
    //          \ |
    // 			 \|
    //  left <- - .

    Vec3 direction = center - eye;

    left = up.cross(direction).normalize();

    this->up = direction.cross(left).normalize();

    this->setAxes(this->left, this->up, direction);
}

void Camera::setAxes(const Vec3 &left, const Vec3 &up, const Vec3 &direction)
{
    Quat quat;
    quat.fromAxes(left, up, direction);

    // TODO: error, not need it
    auto transform = this->getComponent<Transform>();
    transform->setWorldRotation(quat);

    this->onChange();
}

const Mat4 &Camera::getViewMatrix() const
{
    return this->viewMatrix;
}

const Mat4 &Camera::getProjectionMatrix() const
{
    return this->projectionMatrix;
}

const Mat4 &Camera::getViewProjectionMatrix() const
{
    return this->viewProjectionMatrix;
}

RenderTargetPtr Camera::getRenderTarget() const
{
    return this->renderTarget;
}

void Camera::setRenderTarget(RenderTargetPtr target)
{
    this->renderTarget = target;
}

void Camera::setBuffersClearFlag(int flag)
{
    this->clearFlag = flag;
}

int Camera::getBuffersClearFlag() const
{
    return this->clearFlag;
}

void Camera::setClearColor(const Color &color)
{
    this->clearColor = color;
}

const Color &Camera::getClearColor() const
{
    return this->clearColor;
}

void Camera::setQueueCullFunc(std::function<bool (int)> func)
{
    this->queueCullFunc = func;
}

std::function<bool (int queueID)> Camera::getQueueCullFunc() const
{
    return this->queueCullFunc;
}

void Camera::recalcViewMatrix()
{
    auto transform = this->getComponent<Transform>();

    this->viewMatrix.lookAt(transform->getWorldMatrix().getTranslation(), this->center, this->up);

    return;
}

void Camera::recalcProjectionMatrix()
{
    if (this->mode == CameraProjectionMode::Perspective) {

        float aspect = viewport.getWidth() / viewport.getHeight();

        this->projectionMatrix.setPerspectiveY(fov, aspect, zNear, zFar);
    } else {
        Size ortho = getOrthoSize();

        this->projectionMatrix.setOrthoFrustum(-ortho.width / 2.0, ortho.width / 2.0,
                                               -ortho.height / 2.0, ortho.height / 2.0,
                                               zNear, zFar);
    }

}

void Camera::onChange()
{
    this->getNode()->refreshTransformInHierarchy();

    this->recalcViewMatrix();
    this->recalcProjectionMatrix();
}

}
