#ifndef COMPONENT_H
#define COMPONENT_H

#include "platform.h"

#include "Base/Named.h"
#include "Base/Uncopyable.h"
#include "Base/Clonable.h"

namespace re {

enum class ComponentType {
    Mesh,
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

    NodePtr getNode();

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

} //namespace re

#endif // COMPONENT_H
