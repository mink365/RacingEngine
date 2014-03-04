/*
 * Node.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef NODE_H_
#define NODE_H_

#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"

#include <string>
#include <vector>

#include "platform.h"

class Node {
    friend class FbxParser;
    friend class SceneManager;
public:
	Node();
//    Node(const Node &a);
	virtual ~Node();

    const reVec3 &getLocalTranslation() const;
    void setLocalTranslation(const reVec3 &value);

    const reVec3 &getLocalScaling() const;
    void setLocalScaling(const reVec3 &value);

    const reQuat &getLocalRotation() const;
    void setLocalRotation(const reQuat &value);

    const reMat4& getLocalMatrix() const;

    const reMat4& getWorldMatrix() const;

    Node *getParent() const;
    void setParent(Node *value);

    int getLevel() const;

    const std::vector<Node *> getChildren() const;
    void addChild(Node *node);

protected:
    void setWorldTranslation(const reVec3 &t);
    void setWorldRotation(const reQuat &r);

    /**
     * @brief calcLocalTransformFromWorld
     * 直接使用世界状态反推本地状态
     * 忽略了scale，因为scale和rotate信息混合在了一起，无法分离。
     * 在大多数引擎，如doom3中，模型是没有scale信息的
     */
    void calcLocalTransformFromWorld();

    void updateLocalMatrix();
    void updateWorldMatrix();

    void markLocalTransformRefreshFlag();
    void markWorldTransformRefreshFlag();

    void updateTransform();
    void updateChildrenTransform();

protected:
    std::string name;
    OBJ_ID id;
    int level;

    Node *parent;
    std::vector<Node*> children;

    reVec3 localTranslation;
    reVec3 localScaling;
    reQuat localRotation;

    reMat4 localMatrix;
    reMat4 worldMatrix;

    int refreshFlags;

private:
    static const int RF_LOCAL_TRANSFORM = 0x01;
    static const int RF_WORLD_TRANSFORM = 0x02;
};

#endif /* NODE_H_ */
