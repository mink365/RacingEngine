#include "Entity.h"
#include "Component.h"
#include "Transform.h"
#include "UI/Base/Transform2D.h"
#include "Node.h"

namespace re {

Entity::Entity()
{
    this->transform = std::make_shared<Transform>();

    this->addComponent(transform);
}

TransformPtr& Entity::getTransform()
{
    return transform;
}

const TransformPtr &Entity::getTransform() const
{
    return transform;
}

void Entity::resetTransform(TransformPtr& trans)
{
    RE_ASSERT(trans->getEntity() == this->shared_from_this());

    this->transform = trans;
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
    EntityPtr ptr = this->shared_from_this();

    component->attachEntity = ptr;
    this->components.push_back(component);

    std::type_index id = std::type_index(typeid(*component.get()));
    if (this->componentMap.count(id) == 0) {
        auto& list = this->componentMap[id];
        list.push_back(component);
    } else {
        this->componentMap[id].push_back(component);
    }

    if (ptr->getNode()->isHasParent()) {
        component->onEnter();
    }
}

void Entity::clearComponents()
{
    if (this->getNode()->isInScene()) {
        for (auto& comp : components) {
            comp->onExit();
        }
    }

    this->components.clear();
    this->componentMap.clear();
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
    return std::make_shared<Entity>();
}

void Entity::copyProperties(const Entity *entity)
{
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
        this->transform = std::dynamic_pointer_cast<Transform>(getComponent<ui::Transform2D>());
    }
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

//void Entity::Start()
//{
//    for (auto& component : components) {
//        component->start();
//    }
//}

} // namespace re

