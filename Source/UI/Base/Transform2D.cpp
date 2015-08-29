/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "Transform2D.h"

namespace re {
namespace ui {

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

void Transform2D::setScale(float v)
{
    this->setScale(Vec2(v, v));
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

void Transform2D::setAnchor(const Vec2 &v)
{
    if (v == this->anchor) {
        return;
    }

    this->anchor = v;

    this->anchorInPixel = Vec2(size.width * anchor.x, size.height * anchor.y);

    markLocalTransformRefreshFlag();
}

const Vec2& Transform2D::getAnchor() const
{
    return this->anchor;
}

void Transform2D::setAnchorInPixel(const Vec2 &v)
{
    if (v == this->anchorInPixel) {
        return;
    }

    this->anchorInPixel = v;
    this->anchor = Vec2(anchorInPixel.x / size.width, anchorInPixel.y / size.height);

    markLocalTransformRefreshFlag();
}

Vec2 Transform2D::getAnchorInPixel() const
{
    float x = this->anchor.x * size.width;
    float y = this->anchor.y * size.height;

    return Vec2(x, y);
}

void Transform2D::setSize(const Size &size)
{
    this->size = size;

    this->anchorInPixel = Vec2(size.width * anchor.x, size.height * anchor.y);

    markLocalTransformRefreshFlag();
}

const Size& Transform2D::getSize() const
{
    return this->size;
}

void Transform2D::setWidth(float v)
{
    this->size.width = v;
}

void Transform2D::setHeight(float v)
{
    this->size.height = v;
}

Rect Transform2D::getBoundingBox() const
{
    Rect rect = Rect(0, 0, size.width, size.height);
    return RectApplyMatrix(rect, localMatrix);
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
    const Vec2& skew = this->skew;

    bool needsSkewMatrix = (skew != Vec2::Zero);

    Vec2 anchorPoint;
    const Vec2& anchorInPixel = this->anchorInPixel;
    anchorPoint.x = anchorInPixel.x * localScaling.x;
    anchorPoint.y = anchorInPixel.y * localScaling.y;

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
        if (!(anchorInPixel == Vec2::Zero))
        {
            // XXX: Argh, Mat4 needs a "translate" method.
            // XXX: Although this is faster than multiplying a vec4 * mat4
            localMatrix[3][0] += localMatrix[0][0] * -anchorInPixel.x + localMatrix[1][0] * -anchorInPixel.y;
            localMatrix[3][1] += localMatrix[0][1] * -anchorInPixel.x + localMatrix[1][1] * -anchorInPixel.y;
        }
    }

    this->refreshFlags &= ~RF_LOCAL_TRANSFORM;
    this->markWorldTransformRefreshFlag();
}

ComponentPtr Transform2D::createCloneInstance() const
{
    return CreateCloneInstance<Transform2D>();
}

void Transform2D::copyProperties(const Component *component)
{
    Transform::copyProperties(component);

    const Transform2D* inst = static_cast<const Transform2D*>(component);
    if (inst) {
        this->size = inst->size;
        this->anchor = inst->anchor;
        this->anchorInPixel = inst->anchorInPixel;
        this->skew = inst->skew;
    }
}

} // namespace ui
} // namespace re

