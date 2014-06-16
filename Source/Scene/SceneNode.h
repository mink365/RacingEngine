/*
 * SceneNode.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef SCENENODE_H_
#define SCENENODE_H_

#include "Node.h"
#include "NodeAttribute.h"

namespace re {

class SceneNode;
typedef shared_ptr<SceneNode> SceneNodePtr;

class SceneNode : public Node {
    friend class SceneManager;

public:
	SceneNode();
	virtual ~SceneNode();

    bool getVisible() const;
    void setVisible(bool value);

    void setNodeAttribute(NodeAttributePtr att);
    void clearNodeAttribute();
    NodeAttributePtr getNodeAttribute();

    SceneNodePtr clone();

protected:
    virtual NodePtr createCloneInstance() override;
    virtual void copyProperties(Node* node) override;

protected:
    bool visible;

    NodeAttributePtr attribute;
};

} // namespace re

#endif /* SCENENODE_H_ */
