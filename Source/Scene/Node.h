/*
 * Node.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <typeinfo>
#include <typeindex>
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <map>

#include "platform.h"
#include "Base/Named.h"
#include "Base/Uncopyable.h"
#include "Base/Clonable.h"

namespace re {

class Node;
typedef std::shared_ptr<Node> NodePtr;

class Node : public enable_shared_from_this<Node>, public Named, public Clonable<Node> {
    friend class FbxParser;
public:
	Node();
	virtual ~Node();

    virtual void init();

    NodePtr getParent() const;
    void setParent(NodePtr value);
    void removeFromeParent();
    void resetParent();
    bool isHasParent();

    int getLevel() const;

    const std::vector<NodePtr>& getChildren() const;
    NodePtr getChild(uint32_t index);
    NodePtr getChild(const std::string& name);

    virtual void addChild(NodePtr node, Int index=-1);
    virtual void removeChild(NodePtr node);
    virtual void removeAllChildren();

    void addComponent(ComponentPtr component);
    void clearComponent();
    size_t getComponentCount() const;
    ComponentPtr getComponent(size_t index);
    const std::vector<ComponentPtr>& getComponents() const;

    template<typename T>
    std::shared_ptr<T> getComponent();

    void updateTransform();
    TransformPtr& getTransform();
    const TransformPtr& getTransform() const;

    NodePtr clone() const;

    bool isVisible() const {return this->_visible;};
    void setVisible(bool v) {this->_visible = v;};

protected:
    void updateChildrenTransform();

    virtual NodePtr createCloneInstance() const;
    virtual void copyChildren(const Node* node);
    virtual void copyProperties(const Node *node);
    virtual void copyComponents(const Node *node);

protected:
    int level;

    std::weak_ptr<Node> parent;
    std::vector<NodePtr> children;

    std::vector<ComponentPtr> components;
    std::unordered_map<std::type_index, std::vector<ComponentPtr>> componentMap;

protected:
    TransformPtr transform;

    bool _visible;
};

template<typename T>
inline std::shared_ptr<T> Node::getComponent()
{
    auto iter = this->componentMap.find(std::type_index(typeid(T)));

    if (iter != componentMap.end() && iter->second.size() > 0) {
        return std::static_pointer_cast<T>(iter->second[0]);
    }

    return nullptr;
}

} // namespace re

#endif /* NODE_H_ */
