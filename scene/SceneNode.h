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

enum SceneNodeType {
    GROUP,
    MESH,
    CAMERA,
    LIGHT
};

class SceneNode;
typedef shared_ptr<SceneNode> SceneNodePtr;

class SceneNode : public Node {
    friend class SceneManager;

public:
	SceneNode();
	virtual ~SceneNode();

    bool getVisible() const;
    void setVisible(bool value);

    SceneNodeType getType();
protected:
    bool visible;
    SceneNodeType type;
};

} // namespace re

#endif /* SCENENODE_H_ */
