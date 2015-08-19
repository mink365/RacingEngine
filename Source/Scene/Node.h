#ifndef NODE_H_
#define NODE_H_

#include <vector>

#include "Component.h"

namespace re {

class Node : public Component, public std::enable_shared_from_this<Node>
{
    friend class SceneManager;
    friend class Entity;
    friend class FbxParser;
public:
	Node();

    NodePtr getParent() const;
    void setParent(NodePtr value);
    void removeFromParent();
    void resetParent();
    bool isHasParent();

    int getLevel() const;

    const std::vector<NodePtr>& getChildren() const;
    NodePtr getChild(uint32_t index);
    NodePtr getChild(const std::string& name);

    virtual void addChild(NodePtr node, Int index=-1);
    virtual void removeChild(NodePtr node);
    virtual void removeAllChildren();

    bool isVisible() const {return this->_visible;};
    void setVisible(bool v) {this->_visible = v;};

    bool isInScene() const;

protected:
    virtual ComponentPtr createCloneInstance() const override;
    virtual void copyProperties(const Component* component) override;

protected:
    void OnEnter();
    void OnExit();

protected:
    int level;

    std::weak_ptr<Node> parent;
    std::vector<NodePtr> children;

protected:
    bool _visible = true;
    bool _inScene = false;
};

inline bool Node::isInScene() const
{
    return this->_inScene;
}

template<typename T>
inline std::shared_ptr<T> Entity::getComponentInParent()
{
    auto parent = this->node->getParent();

    if (parent != nullptr) {
        return parent->getEntity()->getComponent<T>();
    }

    return nullptr;
}

} // namespace re

#endif /* NODE_H_ */
