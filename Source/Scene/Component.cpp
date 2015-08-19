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
    return this->getComponent<Node>();
}

TransformPtr Component::getTransform()
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
    return std::make_shared<Component>();
}

void Component::copyProperties(const Component *att)
{
    this->attachEntity.reset();
}

}
