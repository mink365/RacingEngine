#include "Component.h"
#include "Node.h"

namespace re {

Component::Component()
{
}

Component::~Component()
{

}

NodePtr Component::getNode() const
{
    return this->getEntity()->getNode();
}

TransformPtr Component::getTransform() const
{
    return this->getEntity()->getTransform();
}

const std::vector<ComponentPtr> &Component::getComponents() const
{
    return this->getEntity()->getComponents();
}

ComponentPtr Component::clone() const
{
    ComponentPtr cloned = this->createCloneInstance();

    cloned->copyProperties(this);

    return cloned;
}

ComponentPtr Component::createCloneInstance() const
{
    return CreateCloneInstance<Component>();
}

void Component::copyProperties(const Component *att)
{
    this->attachEntity.reset();
}

}
