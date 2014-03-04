#include "Camera.h"

Camera::Camera()
{
}

void Camera::setDepthField(float near, float far)
{
    this->zNear = near;
    this->zFar = far;
}

void Camera::setViewport(float width, float height)
{
    this->viewportWidth = width;
    this->viewportHeight = height;
}

void Camera::setView(const reVec3 &center, const reVec3 &eye, const reVec3 &up)
{
    this->center = center;

    // TODO: 世界/World位置
    this->localTranslation = eye;

    //       dir  up
    //         ^  ^
    //          \ |
    // 			 \|
    //  left <- - .

    reVec3 direction = center - eye;

    left = up.cross(direction).normalize();

    this->up = direction.cross(left).normalize();

    this->setAxes(this->left, this->up, direction);
}

void Camera::setAxes(const reVec3 &left, const reVec3 &up, const reVec3 &direction)
{
    reQuat quat;
    quat.fromAxes(left, up, direction);

    this->setWorldRotation(quat);

    this->onChange();
}

const reMat4 &Camera::getViewMatrix() const
{
    return this->viewMatrix;
}

const reMat4 &Camera::getProjectionMatrix() const
{
    return this->projectionMatrix;
}

const reMat4 &Camera::getViewProjectionMatrix() const
{
    return this->viewProjectionMatrix;
}

void Camera::recalcViewMatrix()
{
    // TODO: 使用world的translation
    this->viewMatrix.lookAt(this->localTranslation, this->center, this->up);
}

void Camera::recalcProjectionMatrix()
{
    // TODO: 区分perspect和ortho相机，只有ortho相机有fovy(张角)
    float aspect = this->viewportWidth / this->viewportHeight;
    this->projectionMatrix.setPerspective(45, zNear, zFar);
}

void Camera::onChange()
{
    this->recalcViewMatrix();
    this->recalcProjectionMatrix();
}
