#include "Camera.h"

namespace re {

Camera::Camera()
{
    this->queueCullFunc = [](int){return false;};
    this->clearFlag = 0;

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

void Camera::setView(const Vec3 &center, const Vec3 &eye, const Vec3 &up)
{
    this->center = center;

    if (this->getParent()) {
        // TODO: right?
        this->localTranslation = this->getParent()->getWorldMatrix().inverse() * center;
    } else {
        this->localTranslation = eye;
    }

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

    this->setWorldRotation(quat);

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

void Camera::setClearFlag(int flag)
{
    this->clearFlag = flag;
}

int Camera::getClearFlag() const
{
    return this->clearFlag;
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
    this->viewMatrix.lookAt(this->worldMatrix.getTranslation(), this->center, this->up);

    return;
}

void Camera::recalcProjectionMatrix()
{
    if (this->mode == CameraProjectionMode::Perspective) {
        this->projectionMatrix.setPerspective(fov, zNear, zFar);
    } else {
        Size ortho = getOrthoSize();

        this->projectionMatrix.setOrthoFrustum(-ortho.width / 2.0, ortho.width / 2.0,
                                               -ortho.height / 2.0, ortho.height / 2.0,
                                               zNear, zFar);
    }

}

void Camera::onChange()
{
    this->updateTransform();

    this->recalcViewMatrix();
    this->recalcProjectionMatrix();
}

}