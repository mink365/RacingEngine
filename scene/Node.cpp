/*
 * Node.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Node.h"

Node::Node() {
    this->id = 0;
    this->level = 0;
    this->refreshFlags = 0;
    this->markLocalTransformRefreshFlag();

    this->localTranslation.set(0, 0, 0);
    this->localRotation.set(0, 0, 0, 1);
    this->localScaling.set(1, 1, 1);
}

//Node::Node(const Node &a)
//{
//    this->name = a.name;
//    this->id = a.id;
//    this->level = 0;
//    this->parent = NULL;
//    this->refreshFlags = 0;
//    this->markLocalTransformRefreshFlag();
//    this->localTranslation = a.localTranslation;
//    this->localScaling = a.localScaling;
//    this->localRotation = a.localRotation;
//    this->localMatrix = a.localMatrix;
//    this->worldMatrix.identity();

//    std::vector<Node *>::iterator iter;
//    for (iter = a.children.begin(); iter != a.children.end(); ++iter) {
//        Node *source = *iter;

//        Node *node = source->clone();
//        this->children.push_back();
//    }
//}

Node::~Node() {

}

const reVec3 &Node::getLocalTranslation() const
{
    return localTranslation;
}

void Node::setLocalTranslation(const reVec3 &value)
{
    localTranslation = value;

    this->markLocalTransformRefreshFlag();
}

const reVec3 &Node::getLocalScaling() const
{
    return localScaling;
}

void Node::setLocalScaling(const reVec3 &value)
{
    localScaling = value;

    this->markLocalTransformRefreshFlag();
}

const reQuat &Node::getLocalRotation() const
{
    return localRotation;
}

void Node::setLocalRotation(const reQuat &value)
{
    localRotation = value;

    this->markLocalTransformRefreshFlag();
}

const reMat4 &Node::getLocalMatrix() const
{
    return this->localMatrix;
}

const reMat4 &Node::getWorldMatrix() const
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

void Node::setWorldTranslation(const reVec3 &t)
{
}

void Node::setWorldRotation(const reQuat &r)
{
}

void Node::calcLocalTransformFromWorld()
{

}
