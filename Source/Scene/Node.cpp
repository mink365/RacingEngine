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

    this->transform = std::make_shared<Transform>();

    this->addComponent(transform);
}

Node::~Node() {

}

void Node::updateTransform()
{
    transform->refresh();

    this->updateChildrenTransform();
}

TransformPtr& Node::getTransform()
{
    return transform;
}

const TransformPtr &Node::getTransform() const
{
    return transform;
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
    for (auto& child : node->children) {
        NodePtr copyChild = child->clone();
        this->children.push_back(copyChild);
        copyChild->parent = this->shared_from_this();
    }
}

void Node::copyProperties(const Node *node)
{
    this->name = node->name;

    this->parent.reset();
}

void Node::copyComponents(const Node *node)
{
    this->components.clear();
    for (auto& component : node->components) {
        ComponentPtr copyComponent = component->clone();
        this->components.push_back(copyComponent);
        copyComponent->attachNode = this->shared_from_this();
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

void Node::addComponent(ComponentPtr component)
{
    NodePtr ptr = this->shared_from_this();

    component->attachNode = ptr;
    this->components.push_back(component);
}

void Node::clearComponent()
{
    this->components.clear();
}

size_t Node::getComponentCount() const
{
    return this->components.size();
}

ComponentPtr Node::getComponent(size_t index)
{
    return this->components.at(index);
}

const std::vector<ComponentPtr> &Node::getComponents() const
{
    return this->components;
}

NodePtr Node::clone() const
{
    NodePtr cloned = this->createCloneInstance();
    cloned->copyProperties(this);
    cloned->copyChildren(this);

    return cloned;
}

}
