#ifndef NODE_H_
#define NODE_H_

#include <vector>

#include "Base/ECS/Component.h"

namespace re {

void DistpatchFunctionInHierarchy(NodePtr& root, std::function<void(NodePtr&)> func);
bool DistpatchFunctionToTop(NodePtr& node, std::function<bool(NodePtr&)> func);

class Node : public Component<Node>
{
    friend class SceneManager;
    friend class Entity;
    friend class FbxParser;
    template<class T>
    friend class Component;

public:
	Node();

    NodePtr getParent() const;
    void setParent(NodePtr value);
    void removeFromParent();
    void resetParent();
    bool hasParent();

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
    virtual void copyProperties(const Node& component) override;

protected:
    void OnEnter();
    void OnExit();

protected:
    int level;

    ComponentHandle<Node> parent;
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
inline ComponentHandle<T> Entity::getComponentInParent()
{
    auto parent = this->node->getParent();

    if (parent != nullptr) {
        return parent->getEntity()->getComponent<T>();
    }

    return nullptr;
}

} // namespace re

#endif /* NODE_H_ */
