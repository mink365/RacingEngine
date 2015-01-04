/*
 * Node.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Node.h"
#include <algorithm>

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

NodePtr Node::createCloneInstance() const
{
    return std::make_shared<Node>();
}

void Node::copyChildren(const Node *node)
{
    this->children.clear();
    for (auto child : node->children) {
        NodePtr copyChild = child->clone();
        this->children.push_back(copyChild);
        copyChild->parent = this->shared_from_this();
    }
}

void Node::copyProperties(const Node *node)
{
    this->name = node->name;
    this->localRotation = node->localRotation;
    this->localTranslation = node->localTranslation;
    this->localScaling = node->localScaling;

    this->localMatrix = node->localMatrix;
    this->worldMatrix = node->worldMatrix;

    this->markLocalTransformRefreshFlag();
    this->markWorldTransformRefreshFlag();

    this->parent.reset();
}

NodePtr Node::getParent() const
{
    return parent.lock();
}

void Node::setParent(NodePtr value)
{
    parent = value;
}

void Node::removeFromeParent()
{
    assert(isHasParent());

    auto ins = this->shared_from_this();

    this->parent.lock()->removeChild(ins);
}

void Node::resetParent() {
    parent.reset();
}

bool Node::isHasParent() {
    return parent.lock() != nullptr;
}

int Node::getLevel() const
{
    return this->level;
}

const std::vector<NodePtr>& Node::getChildren() const
{
    return children;
}

NodePtr Node::getChild(uint32_t index)
{
    return children.at(index);
}

NodePtr Node::getChild(const string &name)
{
    auto iter = std::find_if(children.begin(), children.end(), [&](const NodePtr& node){
        if (node->getName() == name) {
            return true;
        }

        return false;
    });

    if (iter != children.end()) {
        return *iter;
    }

    return nullptr;
}

void Node::addChild(NodePtr node, Int index)
{
    node->parent = this->shared_from_this();
    node->level = node->getParent()->level + 1;

    vector<NodePtr>::iterator iter;
    if (index >= 0) {
        Int _index = index > children.size() ? children.size() : index;
        iter = children.begin() + _index;
    } else {
        Int _index = index + children.size() + 1;
        _index = _index < 0 ? 0 : _index;
        iter = children.begin() + _index;
    }

    this->children.insert(iter, node);
}

void Node::removeChild(NodePtr node)
{
    auto iter = std::find(children.begin(), children.end(), node);

    if (iter != children.end()) {
        this->children.erase(iter);
        node->parent.reset();
    }
}

void Node::removeAllChildren()
{
    for (auto child : children) {
        child->parent.reset();
    }

    this->children.clear();
}

NodePtr Node::clone() const
{
    NodePtr cloned = this->createCloneInstance();
    cloned->copyProperties(this);
    cloned->copyChildren(this);

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
