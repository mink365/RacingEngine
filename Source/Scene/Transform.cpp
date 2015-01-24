/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "Transform.h"

#include "Node.h"
#include "Component.h"

namespace re {

Transform::Transform()
{
    this->type = ComponentType::Transform;

    this->refreshFlags = 0;
    this->markLocalTransformRefreshFlag();

    this->localTranslation.set(0, 0, 0);
    this->localRotation.set(0, 0, 0, 1);
    this->localScaling.set(1, 1, 1);
}

Transform::~Transform()
{

}

const Vec3& Transform::getLocalTranslation() const
{
    return localTranslation;
}

void Transform::setLocalTranslation(const Vec3 &value)
{
    localTranslation = value;

    this->markLocalTransformRefreshFlag();
}

const Vec3& Transform::getLocalScaling() const
{
    return localScaling;
}

void Transform::setLocalScaling(const Vec3 &value)
{
    localScaling = value;

    this->markLocalTransformRefreshFlag();
}

const Quat& Transform::getLocalRotation() const
{
    return localRotation;
}

void Transform::setLocalRotation(const Quat &value)
{
    localRotation = value;

    this->markLocalTransformRefreshFlag();
}

const Mat4& Transform::getLocalMatrix() const
{
    return this->localMatrix;
}

const Mat4& Transform::getWorldMatrix() const
{
    return this->worldMatrix;
}

TransformPtr Transform::getParentTransform() const
{
    NodePtr parent = this->getNode()->getParent();
    if (parent != nullptr)
    {
        return parent->getTransform();
    }

    return nullptr;
}

void Transform::refresh()
{
    if (this->refreshFlags & RF_LOCAL_TRANSFORM) {
        this->updateLocalMatrix();
    }

    this->updateWorldMatrix();

    this->transformRefresh.emit();
}

void Transform::setWorldTranslation(const Vec3 &t)
{
}

void Transform::setWorldRotation(const Quat &r)
{
}

void Transform::calcLocalTransformFromWorld()
{

}

void Transform::updateLocalMatrix()
{
    this->localMatrix.fromRTS(localRotation, localTranslation, localScaling);

    this->refreshFlags &= ~RF_LOCAL_TRANSFORM;

    this->markWorldTransformRefreshFlag();
}

void Transform::updateWorldMatrix()
{
    TransformPtr parentTransform = this->getParentTransform();
    if (parentTransform != nullptr) {
        this->worldMatrix = parentTransform->worldMatrix * this->localMatrix;
    } else {
        this->worldMatrix = this->localMatrix;
    }

    this->refreshFlags &= ~RF_WORLD_TRANSFORM;
}

void Transform::markLocalTransformRefreshFlag()
{
    this->refreshFlags |= RF_LOCAL_TRANSFORM;
}

void Transform::markWorldTransformRefreshFlag()
{
    this->refreshFlags |= RF_WORLD_TRANSFORM;
}

ComponentPtr Transform::createCloneInstance() const
{
    return std::make_shared<Transform>();
}

void Transform::copyProperties(const Component *component)
{
    Component::copyProperties(component);

    const Transform* inst = dynamic_cast<const Transform*>(component);
    if (inst) {
        this->localRotation = inst->localRotation;
        this->localTranslation = inst->localTranslation;
        this->localScaling = inst->localScaling;

        this->localMatrix = inst->localMatrix;
        this->worldMatrix = inst->worldMatrix;

        this->markLocalTransformRefreshFlag();
        this->markWorldTransformRefreshFlag();
    }
}

} // namespace re

