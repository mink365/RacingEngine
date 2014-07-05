#include "Node2d.h"

#include "Scene/Mesh.h"

namespace re {

Node2d::Node2d()
{
}

Node2d::ptr Node2d::getParent() const
{
    return dynamic_pointer_cast<Node2d>(SceneNode::getParent());
}

void Node2d::removeFromParent()
{
    if (this->getParent()) {
        this->getParent()->removeChild(this->shared_from_this());
    }
}

void Node2d::onEnter()
{

}

void Node2d::onExit()
{

}

bool Node2d::isRunning()
{

}

bool Node2d::isVisible()
{
    return this->getVisible();
}

void Node2d::stopAllActions()
{

}

void Node2d::setPosition(const Vec2 &position)
{
    this->localTranslation.set(position.x, position.y, this->localTranslation.z);
}

Vec2 Node2d::getPosition() const
{
    return Vec2(localTranslation.x, localTranslation.y);
}

void Node2d::setPosition(float x, float y)
{
    localTranslation.x = x;
    localTranslation.y = y;
}

void Node2d::setPositionX(float v)
{
    localTranslation.x = v;
}

void Node2d::setPositionY(float v)
{
    localTranslation.y = v;
}

float Node2d::getPositionX() const
{
    return localTranslation.x;
}

float Node2d::getPositionY() const
{
    return localTranslation.y;
}

void Node2d::setScale(const Vec2 &scale)
{
    localScaling.x = scale.x;
    localScaling.y = scale.y;
}

Vec2 Node2d::getScale() const
{
    return Vec2(localScaling.x, localScaling.y);
}

void Node2d::setScale(float x, float y)
{
    localScaling.x = x;
    localScaling.y = y;
}

void Node2d::setScale(float v)
{
    this->setScale(v, v);
}

void Node2d::setScaleX(float v)
{
    localScaling.x = v;
}

void Node2d::setScaleY(float v)
{
    localScaling.y = v;
}

float Node2d::getScaleX() const
{
    return localScaling.x;
}

float Node2d::getScaleY() const
{
    return localScaling.y;
}

void Node2d::setRotation(float v)
{
    Vec3 r(0, 0, v * DEG_TO_RAD);
    localRotation.fromAngles(r);
}

float Node2d::getRotation() const
{
    return localRotation.toVec3().z * RAD_TO_DEG;
}

void Node2d::setAnchorPoint(const Vec2 &v)
{
    this->anchorPoint = v;
}

const Vec2 &Node2d::getAnchorPoint() const
{
    return this->anchorPoint;
}

Vec2 Node2d::getAnchorPointInPixels() const
{
    float x = this->anchorPoint.x * size.width;
    float y = this->anchorPoint.y * size.height;

    return Vec2(x, y);
}

void Node2d::setContentSize(const Size &size)
{
    this->size = size;
}

const Size &Node2d::getContentSize() const
{
    return this->size;
}

Rect Node2d::getBoundingBox() const
{
    // TODO:
}

Vec2 Node2d::convertToNodeSpace(const Vec2 &worldPoint) const
{

}

Vec2 Node2d::convertToWorldSpace(const Vec2 &nodePoint) const
{

}

Geometry::ptr Node2d::getGeometry() const
{
    if (this->attribute != nullptr && this->attribute->getType() == NodeAttributeType::Mesh) {
        auto mesh = std::dynamic_pointer_cast<Mesh>(this->attribute);

        return mesh->getGeometry();
    }

    return nullptr;
}

void Rgba::setAlpha(float a)
{
    this->color.a = a;
}

float Rgba::getAlpha() const
{
    return this->color.a;
}

void Rgba::setColor(const Color &color)
{
    this->color = color;
}

const Color &Rgba::getColor() const
{
    return this->color;
}

const Color &Rgba::getDisplayColor() const
{
    return this->worldColor;
}

}
