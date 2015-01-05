#include "Component.h"

namespace re {

Component::Component()
{
}

Component::~Component()
{

}

NodePtr Component::getNode() const
{
    return this->attachNode.lock();
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
    this->name = att->name;
    this->type = att->type;

    this->attachNode.reset();
}

}
