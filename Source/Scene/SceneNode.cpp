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

void SceneNode::setNodeAttribute(NodeAttributePtr att)
{
    SceneNodePtr ptr = std::dynamic_pointer_cast<SceneNode>(this->shared_from_this());

    att->attachNode = ptr;
    this->attribute = att;
}

void SceneNode::clearNodeAttribute()
{
    if (this->attribute != nullptr) {
        this->attribute->attachNode.reset();

        this->attribute = nullptr;
    }
}

NodeAttributePtr SceneNode::getNodeAttribute()
{
    return this->attribute;
}

SceneNodePtr SceneNode::clone()
{
    NodePtr inst = Node::clone();

    return dynamic_pointer_cast<SceneNode>(inst);
}

NodePtr SceneNode::createCloneInstance()
{
    return std::make_shared<SceneNode>();
}

void SceneNode::copyProperties(Node *node)
{
    Node::copyProperties(node);

    SceneNode* inst = dynamic_cast<SceneNode*>(node);
    if (inst) {
        this->visible = inst->visible;

        if (inst->attribute != nullptr) {
            NodeAttributePtr clonedAtt = inst->attribute->clone();
            this->setNodeAttribute(clonedAtt);
        }
    }
}

}
