/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "Transform2D.h"
#include "Node2d.h"

namespace re {

Transform2D::Transform2D()
{

}

Transform2D::~Transform2D()
{

}

void Transform2D::setPosition(const Vec2 &position)
{
    localTranslation.set(position.x, position.y, localTranslation.z);

    markLocalTransformRefreshFlag();
}

Vec2 Transform2D::getPosition() const
{
    Vec3 p = getLocalTranslation();

    return Vec2(p.x, p.y);
}

void Transform2D::setPosition(float x, float y)
{
    localTranslation.x = x;
    localTranslation.y = y;

    markLocalTransformRefreshFlag();
}

void Transform2D::setPositionX(float v)
{
    localTranslation.x = v;

    markLocalTransformRefreshFlag();
}

void Transform2D::setPositionY(float v)
{
    localTranslation.y = v;

    markLocalTransformRefreshFlag();
}

float Transform2D::getPositionX() const
{
    return localTranslation.x;
}

float Transform2D::getPositionY() const
{
    return localTranslation.y;
}

void Transform2D::setScale(const Vec2 &scale)
{
    localScaling.x = scale.x;
    localScaling.y = scale.y;

    markLocalTransformRefreshFlag();
}

Vec2 Transform2D::getScale() const
{
    return Vec2(localScaling.x, localScaling.y);
}

void Transform2D::setScale(float x, float y)
{
    localScaling.x = x;
    localScaling.y = y;

    markLocalTransformRefreshFlag();
}

void Transform2D::setScale(float v)
{
    this->setScale(v, v);
}

void Transform2D::setScaleX(float v)
{
    localScaling.x = v;

    markLocalTransformRefreshFlag();
}

void Transform2D::setScaleY(float v)
{
    localScaling.y = v;

    markLocalTransformRefreshFlag();
}

float Transform2D::getScaleX() const
{
    return localScaling.x;
}

float Transform2D::getScaleY() const
{
    return localScaling.y;
}

void Transform2D::setRotation(float v)
{
    Vec3 r(0, 0, v * DEG_TO_RAD);
    localRotation.fromAngles(r);

    markLocalTransformRefreshFlag();
}

float Transform2D::getRotation() const
{
    return localRotation.toVec3().z * RAD_TO_DEG;
}

Vec2 Transform2D::convertToNodeSpace(const Vec2 &worldPoint) const
{
    Vec3 v(worldPoint.x, worldPoint.y, 0);

    Vec3 result = this->getWorldMatrix().inverse() * v;

    return Vec2(result.x, result.y);
}

Vec2 Transform2D::convertToWorldSpace(const Vec2 &nodePoint) const
{
    Vec3 v(nodePoint.x, nodePoint.y, 0);

    Vec3 result = this->getWorldMatrix() * v;

    return Vec2(result.x, result.y);
}

Vec2 Transform2D::convertParentToLocalSpace(const Vec2 &point) const
{
    Vec3 v(point.x, point.y, 0);

    Vec3 result = this->getLocalMatrix().inverse() * v;

    return Vec2(result.x, result.y);
}

Vec2 Transform2D::convertNodeToParentSpace(const Vec2 &point) const
{
    Vec3 v(point.x, point.y, 0);

    Vec3 result = this->getLocalMatrix() * v;

    return Vec2(result.x, result.y);
}

void Transform2D::updateLocalMatrix()
{
    Vec3 position = this->localTranslation;
    const Vec2 skew = getNode2D().skew;

    bool needsSkewMatrix = (skew != Vec2::Zero);

    Vec2 anchorPoint;
    Vec2& anchorPointInPoints = getNode2D().anchorPointInPoints;
    anchorPoint.x = anchorPointInPoints.x * localScaling.x;
    anchorPoint.y = anchorPointInPoints.y * localScaling.y;

    Vec3 rotate = localRotation.toVec3();

    // TODO: use quat to make it
    // TODO: _rotationZ_X _rotationZ_Y
    float cx = 1, sx = 0, cy = 1, sy = 0;
    if (rotate.z || rotate.z)
    {
        float radiansX = -rotate.z;
        float radiansY = -rotate.z;
        cx = cosf(radiansX);
        sx = sinf(radiansX);
        cy = cosf(radiansY);
        sy = sinf(radiansY);
    }

    if (!needsSkewMatrix && !(anchorPoint == Vec2::Zero)) {
        position.x += cy * -anchorPoint.x + -sx * -anchorPoint.y;
        position.y += sy * -anchorPoint.x +  cx * -anchorPoint.y;
    }

//    // TODO:
//    this->localMatrix.set(cy * localScaling.x,   sy * localScaling.x,   0,          0,
//                         -sx * localScaling.y,  cx * localScaling.y,   0,          0,
//                         0,              0,              localScaling.z,    0,
//                         position.x,    position.y,    position.z,          1 );

    this->localMatrix.set(cy * localScaling.x,   -sx * localScaling.y,   0,         position.x,
                           sy * localScaling.x,    cx * localScaling.y,   0,          position.y,
                           0,                                0,        localScaling.z,  position.z,
                           0,                                0,               0,            1 );

    // TODO: rotate X and Y

    if (needsSkewMatrix)
    {
        // TODO: .....
        Mat4 skewMatrix(1, (float)tanf(DegreeToRadian(skew.y)), 0, 0,
                          (float)tanf(DegreeToRadian(skew.x)), 1, 0, 0,
                          0,  0,  1, 0,
                          0,  0,  0, 1);

        localMatrix = localMatrix * skewMatrix;

        // adjust anchor point
        if (!(anchorPointInPoints == Vec2::Zero))
        {
            // XXX: Argh, Mat4 needs a "translate" method.
            // XXX: Although this is faster than multiplying a vec4 * mat4
            localMatrix[3][0] += localMatrix[0][0] * -anchorPointInPoints.x + localMatrix[1][0] * -anchorPointInPoints.y;
            localMatrix[3][1] += localMatrix[0][1] * -anchorPointInPoints.x + localMatrix[1][1] * -anchorPointInPoints.y;
        }
    }

    this->refreshFlags &= ~RF_LOCAL_TRANSFORM;
    this->markWorldTransformRefreshFlag();
}

Node2d& Transform2D::getNode2D()
{
    return *static_cast<Node2d*>(getNode().get());
}

} // namespace re

