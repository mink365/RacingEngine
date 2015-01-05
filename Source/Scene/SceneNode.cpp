/*
 * SceneNode.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "SceneNode.h"

namespace re {

SceneNode::SceneNode() {
    this->visible = true;
}

SceneNode::~SceneNode() {

}

bool SceneNode::getVisible() const
{
    return visible;
}

void SceneNode::setVisible(bool value)
{
    visible = value;
}

SceneNodePtr SceneNode::clone()
{
    NodePtr inst = Node::clone();

    return dynamic_pointer_cast<SceneNode>(inst);
}

NodePtr SceneNode::createCloneInstance() const
{
    return std::make_shared<SceneNode>();
}

void SceneNode::copyProperties(const Node *node)
{
    Node::copyProperties(node);

    const SceneNode* inst = dynamic_cast<const SceneNode*>(node);
    if (inst) {
        this->visible = inst->visible;
    }
}

}
