#ifndef COMPONENT_H
#define COMPONENT_H

#include "platform.h"

#include "Base/Clonable.h"
#include "Scene/Entity.h"
#include "Message/Signal.h"
#include "Util/ComponentFactory.h"
#include "Util/EventUtil.h"

namespace re {

class Component : public Clonable<Component>
{
    friend class Entity;
    friend class FbxParser;

public:
    Component();
    virtual ~Component();

    Long getId() const;

    const std::string& getName() const;
    void setName(const std::string& name);

    EntityPtr getEntity() const;
    NodePtr getNode() const;

    TransformPtr getTransform();

    template<typename T>
    SharedPtr<T> getComponent() const;

    template<typename T>
    SharedPtr<T> getComponentInParent();

    const std::vector<ComponentPtr>& getComponents() const;

public:
    virtual void RegisterEvents() {};

    ComponentPtr clone() const;

protected:
    virtual ComponentPtr createCloneInstance() const;
    virtual void copyProperties(const Component *att);

protected:
    std::weak_ptr<Entity> attachEntity;
};

inline Long Component::getId() const
{
    return this->getEntity()->getId();
}

inline const string &Component::getName() const
{
    return this->getEntity()->getName();
}

inline void Component::setName(const string &name)
{
    this->getEntity()->setName(name);
}

inline EntityPtr Component::getEntity() const
{
    return this->attachEntity.lock();
}

template<typename T>
inline SharedPtr<T> Component::getComponent() const
{
    return getEntity()->getComponent<T>();
}

template<typename T>
inline SharedPtr<T> Component::getComponentInParent()
{
    return getEntity()->getComponentInParent<T>();
}

} //namespace re

#endif // COMPONENT_H
