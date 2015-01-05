/*
 * SceneNode.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef SCENENODE_H_
#define SCENENODE_H_

#include "Node.h"

namespace re {

class SceneNode;
typedef shared_ptr<SceneNode> SceneNodePtr;

class SceneNode : public Node {
    friend class SceneManager;

public:
    SceneNode();
    virtual ~SceneNode();

    void init();
    void init(const std::string& name);

    bool getVisible() const;
    void setVisible(bool value);

    SceneNodePtr clone();

protected:
    virtual NodePtr createCloneInstance() const override;
    virtual void copyProperties(const Node* node) override;

protected:
    bool visible;
};

inline void SceneNode::init()
{
}

inline void SceneNode::init(const string &name)
{
    this->name = name;
}

} // namespace re

#endif /* SCENENODE_H_ */
