/*
 * SceneNode.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "SceneNode.h"

namespace re {

SceneNode::SceneNode() {
    this->type = GROUP;
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

SceneNodeType SceneNode::getType()
{
    return this->type;
}

}
