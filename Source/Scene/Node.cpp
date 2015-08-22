#include "Node.h"
#include <algorithm>
#include "Transform.h"
#include "UI/Base/Transform2D.h"

namespace re {

Node::Node() {
    this->level = 0;
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
    assert(hasParent());

    auto ins = this->shared_from_this();
    this->parent.lock()->removeChild(ins);
}

void Node::resetParent() {
    parent.reset();
}

bool Node::hasParent() {
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

    auto func = [](NodePtr& node){
        node->level = node->getParent()->level + 1;
    };
    DistpatchFunctionInHierarchy(node, func);

    vector<NodePtr>::iterator iter;
    if (index >= 0) {
        size_t _index = (size_t)index > children.size() ? children.size() : index;
        iter = children.begin() + _index;
    } else {
        Int _index = index + children.size() + 1;
        _index = _index < 0 ? 0 : _index;
        iter = children.begin() + _index;
    }

    this->children.insert(iter, node);

    if (this->_inScene) {
        node->OnEnter();
    }
}

void Node::removeChild(NodePtr node)
{
    auto iter = std::find(children.begin(), children.end(), node);

    if (iter != children.end()) {
        this->children.erase(iter);
        node->parent.reset();
    }

    if (this->_inScene) {
        node->OnExit();
    }
}

void Node::removeAllChildren()
{
    for (auto& child : children) {
        child->parent.reset();

        if (this->_inScene) {
            child->OnExit();
        }
    }

    this->children.clear();
}

ComponentPtr Node::createCloneInstance() const
{
    return std::make_shared<Node>();
}

void Node::copyProperties(const Component *component)
{
    this->parent.reset();
}

void Node::OnEnter()
{
    auto func = [](NodePtr& node) {
        node->_inScene = true;

        node->getEntity()->enterEvent.emit();
    };

    auto node = this->shared_from_this();
    DistpatchFunctionInHierarchy(node, func);
}

void Node::OnExit()
{
    auto func = [](NodePtr& node) {
        node->_inScene = false;

        node->getEntity()->exitEvent.emit();
    };

    auto node = this->shared_from_this();
    DistpatchFunctionInHierarchy(node, func);
}

void DistpatchFunctionInHierarchy(NodePtr &root, std::function<void (NodePtr &)> func)
{
    func(root);

    for (NodePtr child : root->getChildren()) {
        DistpatchFunctionInHierarchy(child, func);
    }
}

}
