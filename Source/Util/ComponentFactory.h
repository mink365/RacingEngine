#ifndef RE_COMPONENTFACTORY_H
#define RE_COMPONENTFACTORY_H

#include <vector>
#include <string>
#include <iostream>
#include "PreDeclare.h"
#include "Base/Singleton.h"
#include "Scene/Entity.h"

namespace re {

template <class... TL>
typename std::enable_if<sizeof...(TL) == 0>::type AddComponent(EntityPtr&)
{
}

template <class T, class... TL> void AddComponent(EntityPtr& node)
{
    auto component = std::make_shared<T>();
    node->addComponent(component);
    AddComponent<TL... >(node);
}

class ComponentFactory : public Singleton<ComponentFactory>
{
public:
    std::vector<EntityPtr> nodes;
};

EntityPtr CreateEntity();
EntityPtr CreateNode();
EntityPtr CreateNode(const std::string& name);
EntityPtr CreateMeshNode();
EntityPtr CreateSkinningMesh();
EntityPtr CreateBoneNode();

template<typename T, typename... Args>
inline std::shared_ptr<T> CreateNode(Args... args)
{
    auto entity = CreateNode();

    auto component = std::make_shared<T>(args...);

    entity->addComponent(component);

    return component;
}

template<typename T, typename... Args>
inline std::shared_ptr<T> CreateNode2D(Args... args)
{
    auto node = CreateEntity();

    AddComponent<Node, ui::Transform2D, ui::HierarchyColor, ui::LayoutElement, T>(node);

    auto component = node->getComponent<T>();
    component->init(args...);

    return component;
}

} // namespace re

#endif // RE_COMPONENTFACTORY_H
