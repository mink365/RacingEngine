/*
 * Node.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Node.h"
#include <algorithm>
#include "Transform.h"
#include "UI/Base/Transform2D.h"

namespace re {

Node::Node() {
    this->id = 0;
    this->level = 0;
}

Node::~Node() {

}

void Node::init()
{
    this->transform = std::make_shared<Transform>();

    this->addComponent(transform);
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

void Node::resetTransform(TransformPtr& trans)
{
    RE_ASSERT(trans->getNode() == this->shared_from_this());

    this->transform = trans;
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

void Node::removeFromParent()
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
    for (auto& child : children) {
        child->parent.reset();
    }

    this->children.clear();
}

void Node::addComponent(ComponentPtr component)
{
    NodePtr ptr = this->shared_from_this();

    component->attachNode = ptr;
    this->components.push_back(component);

    std::type_index id = std::type_index(typeid(*component.get()));
    if (this->componentMap.count(id) == 0) {
        auto& list = this->componentMap[id];
        list.push_back(component);
    } else {
        this->componentMap[id].push_back(component);
    }
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

void Node::Start()
{
    for (auto& component : components) {
        component->start();
    }
}

void Node::Update()
{
    for (auto& component : components) {
        component->update();
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

    for (auto& component : this->components) {
        std::type_index id = std::type_index(typeid(*component.get()));
        if (this->componentMap.count(id) == 0) {
            auto& list = this->componentMap[id];
            list.push_back(component);
        } else {
            this->componentMap[id].push_back(component);
        }
    }

    this->transform = this->getComponent<Transform>();
    if (this->transform == nullptr) {
        this->transform = std::dynamic_pointer_cast<Transform>(getComponent<Transform2D>());
    }
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

NodePtr Node::clone() const
{
    NodePtr cloned = this->createCloneInstance();
    cloned->copyChildren(this);
    cloned->copyComponents(this);
    cloned->copyProperties(this);

    return cloned;
}

void DistpatchFunctionInHierarchy(NodePtr &root, std::function<void (NodePtr &)> func)
{
    func(root);

    for (NodePtr child : root->getChildren()) {
        DistpatchFunctionInHierarchy(child, func);
    }
}

}
