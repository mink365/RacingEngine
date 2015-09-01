#ifndef RE_COMPONENTFACTORY_H
#define RE_COMPONENTFACTORY_H

#include <vector>
#include <string>
#include <iostream>
#include "Base/Singleton.h"
#include "Base/ECS/Entity.h"

namespace re {

class Node;
namespace ui
{
    class Transform2D;
    class HierarchyColor;
    class LayoutElement;
    class Widget;
}

template <class... TL>
typename std::enable_if<sizeof...(TL) == 0>::type AddComponent(EntityPtr&)
{
}

template <class T, class... TL> void AddComponent(EntityPtr& node)
{
    node->addComponent<T>();
    AddComponent<TL... >(node);
}

EntityPtr CreateEntity();
EntityPtr CreateNodeEntity();
EntityPtr CreateNodeEntity(const std::string& name);
EntityPtr CreateMeshEntity();
EntityPtr CreateSkinningMeshEntity();
EntityPtr CreateBoneEntity();

template<typename T>
ComponentHandle<T> CreateNode()
{
    auto entity = CreateEntity();

    AddComponent<Node, Transform, T>(entity);

    entity->switchState(EntityState::Awaked);

    auto component = entity->getComponent<T>();

    return component;
}

template<typename T>
ComponentHandle<T> CreateNode2D()
{
    auto entity = CreateEntity();

    AddComponent<Node, ui::Transform2D, ui::HierarchyColor, ui::LayoutElement, ui::Widget, T>(entity);

    entity->switchState(EntityState::Awaked);

    auto component = entity->getComponent<T>();

    return component;
}

template<typename T, typename... Args>
ComponentHandle<T> CreateUIGraphicNode(Args... args)
{
    auto entity = CreateEntity();

    AddComponent<Node, ui::Transform2D, ui::HierarchyColor, ui::LayoutElement, T>(entity);

    entity->switchState(EntityState::Awaked);

    auto component = entity->getComponent<T>();
    component->init(args...);

    return component;
}

} // namespace re

#endif // RE_COMPONENTFACTORY_H
