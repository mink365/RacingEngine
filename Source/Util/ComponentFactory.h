#ifndef RE_COMPONENTFACTORY_H
#define RE_COMPONENTFACTORY_H

#include "PreDeclare.h"
#include "Base/Singleton.h"
#include "Scene/Node.h"

namespace re {

class LayoutElement;

class ComponentFactory : public Singleton<ComponentFactory>
{
public:
    std::vector<NodePtr> nodes;
};

NodePtr CreateNode();
NodePtr CreateNode(const string& name);
NodePtr CreateMeshNode();
NodePtr CreateSkinningMesh();
NodePtr CreateBoneNode();

template<typename T, typename... Args>
inline std::shared_ptr<T> CreateComponent(Args... args)
{
    auto node = Create<Node>();

    auto component = std::make_shared<T>();

    node->addComponent(component);

    // make an reference to the node, make sure it not be delected
    ComponentFactory::getInstance().nodes.push_back(node);

    return component;
}

// function can not partial specialization now, so we need this
template<std::size_t INDEX, typename T, typename... TL>
struct AddComponent
{
    static void Do(NodePtr& node)
    {
        auto component = std::make_shared<T>();
        node->addComponent(component);

        AddComponent<INDEX-1, TL...>::Do(node);
    }
};

template<typename T, typename... TL>
struct AddComponent<1, T, TL...>
{
    static void Do(NodePtr& node)
    {
        // the last one
        auto component = std::make_shared<T>();
        node->addComponent(component);
    }
};

template<typename T, typename... Args>
inline std::shared_ptr<T> CreateNode2DComponent(Args... args)
{
    auto node = std::make_shared<Node>();

    AddComponent<4, Transform2D, HierarchyColor, LayoutElement, T>::Do(node);

    auto transform = node->getComponent<Transform2D>();
    // TODO: can't auto convert?
    std::shared_ptr<Transform> trans = std::dynamic_pointer_cast<Transform>(transform);
    node->resetTransform(trans);

    auto component = node->getComponent<T>();
    component->init(args...);

    ComponentFactory::getInstance().nodes.push_back(node);

    return component;
}

} // namespace re

#endif // RE_COMPONENTFACTORY_H
