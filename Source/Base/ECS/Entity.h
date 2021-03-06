#ifndef RE_ENTITY_H
#define RE_ENTITY_H

#include <vector>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <list>
#include "Base/Clonable.h"
#include "Base/Named.h"
#include "Message/Signal.h"
#include "ComponentHandle.h"
#include "EntityManager.h"
#include "Event.h"

namespace re {

class Entity;
class BaseComponent;
class Transform;
class Node;

typedef SharedPtr<Entity> EntityPtr;
typedef ComponentHandle<Transform> TransformPtr;
typedef ComponentHandle<Node> NodePtr;
typedef ComponentHandle<BaseComponent> ComponentPtr;

template<typename C>
struct ComponentAddedEvent
{
    EntityPtr entity;
};

enum class EntityState
{
    Init,
    Awaked,
    Started,
    Destroyed,
};

class Entity : public Named, public Clonable<Entity>, public std::enable_shared_from_this<Entity>
{
    friend class FbxParser;

public:
    Entity();

    void addComponent(ComponentPtr component);

    template<typename T, typename... Args>
    ComponentHandle<T> addComponent(Args... args);

    void clearComponents();

    size_t getComponentCount() const;
    ComponentPtr getComponent(size_t index);
    const std::vector<ComponentPtr>& getComponents() const;

    template<typename T>
    ComponentHandle<T> getComponent();

    template<typename T>
    ComponentHandle<T> getComponentInParent();

    EntityPtr clone() const;

    EntityState getState() const;
    void switchState(EntityState state);

    bool isEnable() const;
    void setEnable(bool value);

public:
    TransformPtr& getTransform();
    const TransformPtr& getTransform() const;

    NodePtr getNode();

public:
    void refreshTransformInHierarchy();

public:
    Signal<void()> awakeEvent;
    Signal<void()> startEvent;
    Signal<void()> enterEvent;
    Signal<void()> exitEvent;
    Signal<void()> enableEvent;
    Signal<void()> disableEvent;
    Signal<void()> destroyEvent;

private:
    void CacheComponents();

private:
    virtual void copyProperties(const Entity *entity);
    virtual void copyChildren(const Entity* entity);
    virtual void copyComponents(const Entity *entity);

private:
    virtual EntityPtr createCloneInstance() const;

private:
    std::vector<ComponentPtr> components;
    // same type component should never be add to one entity
    std::unordered_map<std::type_index, ComponentPtr> componentMap;

    struct CachedData {
        bool dirty = true;
        std::list<ComponentPtr> list;
    };
    std::unordered_map<std::type_index, CachedData> componentCacheMap;

    EntityState state = EntityState::Init;
    bool enabled = false;

    TransformPtr transform;
    NodePtr node;
};

template<typename T, typename... Args>
ComponentHandle<T> Entity::addComponent(Args... args)
{
    auto comp = CreateComponent<T>(args...);
    EventManager::instance().emit(ComponentAddedEvent<T>{this->shared_from_this()});

    this->addComponent(comp);

    return comp;
}

template<typename T>
inline ComponentHandle<T> Entity::getComponent()
{
    auto id = std::type_index(typeid(T));

    if (componentMap.count(id) == 0) {
        auto& data = componentCacheMap[id];

        if (data.dirty) {
            for (auto& comp : components) {
                if (dynamic_cast<T*>(comp.get()))
                {
                    data.list.push_back(comp);
                }
            }

            data.dirty = false;
        }

        if (data.list.size() > 0) {
            SharedPtr<T> ptr = std::static_pointer_cast<T>(data.list.front().getPtr());
            return ComponentHandle<T>(ptr, this->shared_from_this());
        }
    } else {
        SharedPtr<T> ptr = std::static_pointer_cast<T>(componentMap[id].getPtr());
        return ComponentHandle<T>(ptr, this->shared_from_this());
    }

    return nullptr;
}

inline NodePtr Entity::getNode()
{
    return this->node;
}

inline TransformPtr& Entity::getTransform()
{
    return transform;
}

inline const TransformPtr &Entity::getTransform() const
{
    return transform;
}

inline EntityState Entity::getState() const
{
    return state;
}

inline void Entity::switchState(EntityState state)
{
    if (this->state == state) {
        return;
    }

    this->state = state;

    switch(this->state) {
    case EntityState::Init:
        break;
    case EntityState::Awaked:
        this->awakeEvent.emit();
        break;
    case EntityState::Started:
        this->startEvent.emit();
        break;
    case EntityState::Destroyed:
        this->destroyEvent.emit();
        break;
    }
}

inline bool Entity::isEnable() const
{
    return enabled;
}

inline void Entity::setEnable(bool value)
{
    if (this->enabled == value) {
        return;
    }

    this->enabled = value;

    if (enabled) {
        this->enableEvent.emit();
    } else {
        this->disableEvent.emit();
    }
}

} // namespace re

#endif // RE_ENTITY_H
