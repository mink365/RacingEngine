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
	// TODO Auto-generated destructor stub
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

}
