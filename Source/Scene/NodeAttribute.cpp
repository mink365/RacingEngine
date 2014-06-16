#include "NodeAttribute.h"

namespace re {

NodeAttribute::NodeAttribute()
{
}

NodeAttribute::~NodeAttribute()
{

}

SceneNodePtr NodeAttribute::getNode()
{
    return this->attachNode.lock();
}

NodeAttributePtr NodeAttribute::clone()
{
    NodeAttributePtr cloned = this->createCloneInstance();

    cloned->copyProperties(this);

    return cloned;
}

NodeAttributePtr NodeAttribute::createCloneInstance()
{
    return std::make_shared<NodeAttribute>();
}

void NodeAttribute::copyProperties(NodeAttribute *att)
{
    att->name = this->name;
    att->type = this->type;

    this->attachNode.reset();
}

}
