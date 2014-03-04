/*
 * Node.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Node.h"

Node::Node() {
    this->parent = NULL;
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
    if (this->parent != NULL) {
        this->worldMatrix = this->parent->worldMatrix * this->localMatrix;
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
    std::vector<Node *>::iterator iter;
    for (iter = this->children.begin(); iter != this->children.end(); ++iter) {
        Node* child = *iter;

        child->updateTransform();
    }
}

Node *Node::getParent() const
{
    return parent;
}

void Node::setParent(Node *value)
{
    parent = value;
}

int Node::getLevel() const
{
    return this->level;
}

const std::vector<Node *> Node::getChildren() const
{
    return children;
}

void Node::addChild(Node *node)
{
    node->parent = this;
    node->level = node->parent->level + 1;

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
