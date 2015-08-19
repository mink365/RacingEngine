#ifndef COMPONENT_H
#define COMPONENT_H

#include "platform.h"

#include "Base/Clonable.h"
#include "Scene/Entity.h"
#include "Message/Signal.h"
#include "Util/ComponentFactory.h"

namespace re {

enum class ComponentType {
    Transform,
    Camera,
    Light,
};

class Component : public Named, public Clonable<Component>
{
    friend class Entity;
    friend class FbxParser;

public:
    Component();
    virtual ~Component();

    ComponentType getType();

    EntityPtr getEntity() const;
    NodePtr getNode() const;

    TransformPtr getTransform();

    template<typename T>
    std::shared_ptr<T> getComponent() const;

    template<typename T>
    std::shared_ptr<T> getComponentInParent();

    const std::vector<ComponentPtr>& getComponents() const;

public:
    /* fuction of event */
    virtual void onEnter() {};
    virtual void onExit() {};
    virtual void start() {};

    ComponentPtr clone() const;

protected:
    virtual ComponentPtr createCloneInstance() const;
    virtual void copyProperties(const Component *att);

protected:
    ComponentType type;

    std::weak_ptr<Entity> attachEntity;
};

inline ComponentType Component::getType()
{
    return this->type;
}

inline EntityPtr Component::getEntity() const
{
    return this->attachEntity.lock();
}

template<typename T>
inline std::shared_ptr<T> Component::getComponent() const
{
    return getEntity()->getComponent<T>();
}

template<typename T>
inline std::shared_ptr<T> Component::getComponentInParent()
{
    return getEntity()->getComponentInParent<T>();
}

} //namespace re

#endif // COMPONENT_H
