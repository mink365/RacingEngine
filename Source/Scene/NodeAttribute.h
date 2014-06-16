#ifndef NODEATTRIBUTE_H
#define NODEATTRIBUTE_H

#include "platform.h"

namespace re {

enum class NodeAttributeType {
    Mesh,
    Camera,
    Light,
};

class NodeAttribute;
typedef std::shared_ptr<NodeAttribute> NodeAttributePtr;

class NodeAttribute
{
    friend class SceneNode;
    friend class FbxParser;

public:
    NodeAttribute();
    virtual ~NodeAttribute();

    NodeAttributeType getType();

    SceneNodePtr getNode();

    NodeAttributePtr clone();

protected:
    virtual NodeAttributePtr createCloneInstance();
    virtual void copyProperties(NodeAttribute* att);

protected:
    string name;
    Long id;

    NodeAttributeType type;

    std::weak_ptr<SceneNode> attachNode;
};

inline NodeAttributeType NodeAttribute::getType()
{
    return this->type;
}

} //namespace re

#endif // NODEATTRIBUTE_H
