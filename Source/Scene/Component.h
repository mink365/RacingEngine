#ifndef COMPONENT_H
#define COMPONENT_H

#include "platform.h"

#include "Base/Named.h"
#include "Base/Uncopyable.h"
#include "Base/Clonable.h"
#include "Scene/Node.h"
#include "Message/Signal.h"

namespace re {

enum class ComponentType {
    Transform,
    Mesh,
    Material,
    Camera,
    Light,
};

class Component;
typedef std::shared_ptr<Component> ComponentPtr;

class Node;
typedef std::shared_ptr<Node> NodePtr;

class Component : public Named, public Clonable<Component>
{
    friend class Node;
    friend class FbxParser;

public:
    Component();
    virtual ~Component();

    ComponentType getType();

    NodePtr getNode() const;

    template<typename T>
    std::shared_ptr<T> getComponent();

    template<typename T>
    std::shared_ptr<T> getComponentInParent();

    /* fuction of event */
    virtual void onEnter() {};
    virtual void onExit() {};
    virtual void start() {};
    virtual void update() {};

    ComponentPtr clone() const;

protected:
    virtual ComponentPtr createCloneInstance() const;
    virtual void copyProperties(const Component *att);

protected:
    ComponentType type;

    std::weak_ptr<Node> attachNode;
};

inline ComponentType Component::getType()
{
    return this->type;
}

template<typename T>
inline std::shared_ptr<T> Component::getComponent()
{
    return getNode()->getComponent<T>();
}

template<typename T>
inline std::shared_ptr<T> Component::getComponentInParent()
{
    return getNode()->getComponentInParent<T>();
}

} //namespace re

#endif // COMPONENT_H
