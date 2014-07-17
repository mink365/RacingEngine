/*
 * Node.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef NODE_H_
#define NODE_H_

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

#include <string>
#include <vector>
#include <memory>

#include "platform.h"
#include "Base/Named.h"
#include "Base/Uncopyable.h"
#include "Base/Clonable.h"

namespace re {

class Node;
typedef std::shared_ptr<Node> NodePtr;

class Node : public enable_shared_from_this<Node>, public Named, public Clonable<Node> {
    friend class FbxParser;
    friend class SceneManager;
public:
	Node();
	virtual ~Node();

    const Vec3 &getLocalTranslation() const;
    void setLocalTranslation(const Vec3 &value);

    const Vec3 &getLocalScaling() const;
    void setLocalScaling(const Vec3 &value);

    const Quat &getLocalRotation() const;
    void setLocalRotation(const Quat &value);

    const Mat4& getLocalMatrix() const;

    const Mat4& getWorldMatrix() const;

    NodePtr getParent() const;
    void setParent(NodePtr value);
    void resetParent();
    bool isHasParent();

    int getLevel() const;

    const std::vector<NodePtr> getChildren() const;
    void addChild(NodePtr node);
    void removeChild(NodePtr node);
    void removeAllChildren();

    void updateTransform();

    NodePtr clone() const;

protected:
    void setWorldTranslation(const Vec3 &t);
    void setWorldRotation(const Quat &r);

    /**
     * @brief calcLocalTransformFromWorld
     * 直接使用世界状态反推本地状态
     * 忽略了scale，因为scale和rotate信息混合在了一起，无法分离。
     * 在大多数引擎，如doom3中，模型是没有scale信息的
     */
    void calcLocalTransformFromWorld();

    virtual void updateLocalMatrix();
    void updateWorldMatrix();

    void markLocalTransformRefreshFlag();
    void markWorldTransformRefreshFlag();

    void updateChildrenTransform();

    virtual NodePtr createCloneInstance() const;
    virtual void copyChildren(const Node* node);
    virtual void copyProperties(const Node *node);

protected:
    int level;

    std::weak_ptr<Node> parent;
    std::vector<NodePtr> children;

    Vec3 localTranslation;
    Vec3 localScaling;
    Quat localRotation;

    Mat4 localMatrix;
    Mat4 worldMatrix;

    int refreshFlags;

protected:
    static const int RF_LOCAL_TRANSFORM = 0x01;
    static const int RF_WORLD_TRANSFORM = 0x02;
};

} // namespace re

#endif /* NODE_H_ */
