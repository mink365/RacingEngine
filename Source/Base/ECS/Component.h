#ifndef COMPONENT_H
#define COMPONENT_H

#include "platform.h"

#include "Base/Clonable.h"
#include "Entity.h"
#include "Message/Signal.h"
#include "Util/EventUtil.h"

namespace re {

class BaseComponent
{
    friend class Entity;
    friend class FbxParser;

public:
    BaseComponent();
    virtual ~BaseComponent();

    Long getId() const;

    const std::string& getName() const;
    void setName(const std::string& name);

    EntityPtr getEntity() const;
    NodePtr getNode() const;
    TransformPtr getTransform() const;

    template<typename T>
    ComponentHandle<T> getComponent() const;

    template<typename T>
    ComponentHandle<T> getComponentInParent() const;

    const std::vector<ComponentHandle<BaseComponent>>& getComponents() const;

public:
    virtual void registerEvents() {};

protected:
    virtual ComponentHandle<BaseComponent> clone() const = 0;

protected:
    WeakPtr<Entity> attachEntity;
};

template <class Derived>
class Component : public BaseComponent
{
    using Handle = ComponentHandle<Derived>;

protected:
    ComponentHandle<BaseComponent> clone() const override
    {
        return __clone();
    }

private:
    Handle __clone() const
    {
        Handle cloned = this->createCloneInstance();

        cloned->attachEntity.reset();
        // TODO:
//        cloned->copyProperties(*(this));

        return cloned;
    }

protected:
    Handle createCloneInstance() const
    {
        Handle handle = CreateComponent<Derived>();
        return handle;
    }
};

inline Long BaseComponent::getId() const
{
    return this->getEntity()->getId();
}

inline const string &BaseComponent::getName() const
{
    return this->getEntity()->getName();
}

inline void BaseComponent::setName(const string &name)
{
    this->getEntity()->setName(name);
}

inline EntityPtr BaseComponent::getEntity() const
{
    return this->attachEntity.lock();
}

template<typename T>
inline ComponentHandle<T> BaseComponent::getComponent() const
{
    return getEntity()->getComponent<T>();
}

template<typename T>
inline ComponentHandle<T> BaseComponent::getComponentInParent() const
{
    return getEntity()->getComponentInParent<T>();
}

} //namespace re

#endif // COMPONENT_H
