#ifndef RE_COMPONENTFACTORY_H
#define RE_COMPONENTFACTORY_H

#include <vector>
#include <string>
#include "PreDeclare.h"
#include "Base/Singleton.h"
#include "Scene/Entity.h"

namespace re {

class ComponentFactory : public Singleton<ComponentFactory>
{
public:
    std::vector<EntityPtr> nodes;
};

EntityPtr CreateNode();
EntityPtr CreateNode(const std::string& name);
EntityPtr CreateMeshNode();
EntityPtr CreateSkinningMesh();
EntityPtr CreateBoneNode();

template<typename T, typename... Args>
inline std::shared_ptr<T> CreateComponent(Args... args)
{
    auto entity = std::make_shared<Entity>();

    auto component = std::make_shared<T>(args...);

    entity->addComponent(component);

    // make an reference to the node, make sure it not be delected
    ComponentFactory::getInstance().nodes.push_back(entity);

    return component;
}

// function can not partial specialization now, so we need this
template<std::size_t INDEX, typename T, typename... TL>
struct AddComponent
{
    static void Do(EntityPtr& node)
    {
        auto component = std::make_shared<T>();
        node->addComponent(component);

        AddComponent<INDEX-1, TL...>::Do(node);
    }
};

template<typename T, typename... TL>
struct AddComponent<1, T, TL...>
{
    static void Do(EntityPtr& node)
    {
        // the last one
        auto component = std::make_shared<T>();
        node->addComponent(component);
    }
};

template<typename T, typename... Args>
inline std::shared_ptr<T> CreateNode2DComponent(Args... args)
{
    auto node = std::make_shared<Entity>();

    AddComponent<4, ui::Transform2D, ui::HierarchyColor, ui::LayoutElement, T>::Do(node);

    auto transform = node->getComponent<ui::Transform2D>();
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
