/*
 * Node.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Node.h"

namespace re {

Node::Node() {
    this->id = 0;
    this->level = 0;
    this->refreshFlags = 0;
    this->markLocalTransformRefreshFlag();

    this->localTranslation.set(0, 0, 0);
    this->localRotation.set(0, 0, 0, 1);
    this->localScaling.set(1, 1, 1);
}

Node::~Node() {

}

const Vec3 &Node::getLocalTranslation() const
{
    return localTranslation;
}

void Node::setLocalTranslation(const Vec3 &value)
{
    localTranslation = value;

    this->markLocalTransformRefreshFlag();
}

const Vec3 &Node::getLocalScaling() const
{
    return localScaling;
}

void Node::setLocalScaling(const Vec3 &value)
{
    localScaling = value;

    this->markLocalTransformRefreshFlag();
}

const Quat &Node::getLocalRotation() const
{
    return localRotation;
}

void Node::setLocalRotation(const Quat &value)
{
    localRotation = value;

    this->markLocalTransformRefreshFlag();
}

const Mat4 &Node::getLocalMatrix() const
{
    return this->localMatrix;
}

const Mat4 &Node::getWorldMatrix() const
{
    return this->worldMatrix;
}

void Node::updateLocalMatrix()
{
    this->localMatrix.fromRTS(localRotation, localTranslation, localScaling);

    this->refreshFlags &= ~RF_LOCAL_TRANSFORM;

    this->markWorldTransformRefreshFlag();
}

void Node::updateWorldMatrix()
{
    if (this->getParent() != nullptr) {
        this->worldMatrix = this->getParent()->worldMatrix * this->localMatrix;
    } else {
        this->worldMatrix = this->localMatrix;
    }

    this->refreshFlags &= ~RF_WORLD_TRANSFORM;
}

void Node::markLocalTransformRefreshFlag()
{
    this->refreshFlags |= RF_LOCAL_TRANSFORM;
}

void Node::markWorldTransformRefreshFlag()
{
    this->refreshFlags |= RF_WORLD_TRANSFORM;
}

void Node::updateTransform()
{
    if (this->refreshFlags & RF_LOCAL_TRANSFORM) {
        this->updateLocalMatrix();
    }

    this->updateWorldMatrix();

    this->updateChildrenTransform();
}

void Node::updateChildrenTransform()
{
    for (auto iter = this->children.begin(); iter != this->children.end(); ++iter) {
        auto child = *iter;

        child->updateTransform();
    }
}

NodePtr Node::createCloneInstance()
{
    return std::make_shared<Node>();
}

void Node::copyChildren(Node *node)
{
    node->children.clear();
    for (auto child : node->children) {
        node->children.push_back(child->clone());
    }
}

void Node::copyProperties(Node *node)
{
    node->name = this->name;
    node->localMatrix = this->localMatrix;
    node->localRotation = this->localRotation;
    node->localTranslation = this->localTranslation;
    node->localScaling = this->localScaling;

    node->worldMatrix = this->worldMatrix;

    node->markLocalTransformRefreshFlag();
    node->markWorldTransformRefreshFlag();

    node->parent.reset();
}

NodePtr Node::getParent() const
{
    return parent.lock();
}

void Node::setParent(NodePtr value)
{
    parent = value;
}

void Node::resetParent() {
    parent.reset();
}

bool Node::isHasParent() {
    return parent.lock() == nullptr;
}

int Node::getLevel() const
{
    return this->level;
}

const std::vector<NodePtr> Node::getChildren() const
{
    return children;
}

void Node::addChild(NodePtr node)
{
    node->parent = this->shared_from_this();
    node->level = node->getParent()->level + 1;

    this->children.push_back(node);
}

NodePtr Node::clone()
{
    NodePtr cloned = this->createCloneInstance();
    cloned->copyProperties(this);
    cloned->copyProperties(this);

    return cloned;
}

void Node::setWorldTranslation(const Vec3 &t)
{
}

void Node::setWorldRotation(const Quat &r)
{
}

void Node::calcLocalTransformFromWorld()
{

}

}
