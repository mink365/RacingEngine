#include "Component.h"
#include "Node.h"

namespace re {

BaseComponent::BaseComponent()
{
}

BaseComponent::~BaseComponent()
{

}

NodePtr BaseComponent::getNode() const
{
    return this->getEntity()->getNode();
}

TransformPtr BaseComponent::getTransform() const
{
    return this->getEntity()->getTransform();
}

const std::vector<ComponentHandle<BaseComponent> > &BaseComponent::getComponents() const
{
    return this->getEntity()->getComponents();
}

}
