#ifndef NODEATTRIBUTE_H
#define NODEATTRIBUTE_H

#include "platform.h"

#include "Base/Named.h"
#include "Base/Uncopyable.h"
#include "Base/Clonable.h"

namespace re {

enum class NodeAttributeType {
    Mesh,
    Camera,
    Light,
};

class NodeAttribute;
typedef std::shared_ptr<NodeAttribute> NodeAttributePtr;

class NodeAttribute : public Named, public Clonable<NodeAttribute>
{
    friend class SceneNode;
    friend class FbxParser;

public:
    NodeAttribute();
    virtual ~NodeAttribute();

    NodeAttributeType getType();

    SceneNodePtr getNode();

    NodeAttributePtr clone() const;

protected:
    virtual NodeAttributePtr createCloneInstance() const;
    virtual void copyProperties(const NodeAttribute *att);

protected:
    NodeAttributeType type;

    std::weak_ptr<SceneNode> attachNode;
};

inline NodeAttributeType NodeAttribute::getType()
{
    return this->type;
}

} //namespace re

#endif // NODEATTRIBUTE_H
