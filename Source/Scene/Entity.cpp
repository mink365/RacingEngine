#include "Entity.h"
#include "Component.h"
#include "Transform.h"
#include "UI/Base/Transform2D.h"
#include "Node.h"
#include "Util/TypeUtil.h"

namespace re {

Entity::Entity()
{
}

void Entity::refreshTransformInHierarchy()
{
    auto func = [](NodePtr& node) {
        node->getEntity()->getTransform()->refresh();
    };

    auto node = this->getComponent<Node>();
    DistpatchFunctionInHierarchy(node, func);
}

void Entity::addComponent(ComponentPtr component)
{
    std::type_index id = std::type_index(typeid(*component.get()));
    assert(componentMap.count(id) == 0);

    EntityPtr ptr = this->shared_from_this();

    component->attachEntity = ptr;
    this->components.push_back(component);

    this->componentMap[id] = component;

    CacheComponents();

    component->RegisterEvents();

    if (this->node != nullptr && this->node->hasParent()) {
        // TODO:
//        component->onEnter();
    }
}

void Entity::clearComponents()
{
    if (this->getNode()->isInScene()) {
        exitEvent.emit();
    }

    this->components.clear();
    this->componentMap.clear();
    this->componentCacheMap.clear();
}

size_t Entity::getComponentCount() const
{
    return this->components.size();
}

ComponentPtr Entity::getComponent(size_t index)
{
    return this->components.at(index);
}

const std::vector<ComponentPtr> &Entity::getComponents() const
{
    return this->components;
}

EntityPtr Entity::createCloneInstance() const
{
    return CreateEntity();
}

void Entity::copyProperties(const Entity*)
{
    for (auto& component : this->components) {
        std::type_index id = std::type_index(typeid(*component.get()));
        this->componentMap[id] = component;
    }

    this->CacheComponents();
}

void Entity::copyChildren(const Entity *entity)
{
    auto node_ = entity->node;
    node_->children.clear();
    for (auto& child : node_->children) {
        auto copyChild = child->getEntity()->clone();
        node_->children.push_back(copyChild->node);
        copyChild->node->parent = this->node;
    }
}

void Entity::copyComponents(const Entity *entity)
{
    this->components.clear();
    for (auto& component : entity->components) {
        ComponentPtr copyComponent = component->clone();
        this->components.push_back(copyComponent);
        copyComponent->attachEntity = this->shared_from_this();
    }
}

EntityPtr Entity::clone() const
{
    EntityPtr cloned = this->createCloneInstance();
    cloned->copyComponents(this);
    cloned->copyProperties(this);

    return cloned;
}

void Entity::CacheComponents()
{
    for (auto& pair : componentCacheMap) {
        auto& data = pair.second;

        data.dirty = true;
    }

    if (this->node == nullptr) {
        this->node = getComponent<Node>();
    }

    if (this->transform == nullptr) {
        this->transform = this->getComponent<Transform>();
    }
}

} // namespace re

