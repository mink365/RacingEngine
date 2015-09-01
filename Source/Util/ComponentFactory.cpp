#include "ComponentFactory.h"

#include "UI/Base/HierarchyColor.h"
#include "UI/Base/Transform2D.h"
#include "UI/Layout/LayoutElement.h"
#include "UI/Widget.h"
#include "UI/Base/Label.h"
#include "UI/Base/Sprite.h"
#include "UI/Base/NinePatch.h"

#include "Scene/RenderElement.h"
#include "Render/Mesh.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/SkeletonController.h"
#include "Animation/Bone.h"
#include "Animation/AnimationTrack.h"

namespace re {

EntityPtr CreateEntity()
{
    auto node = Create<Entity>();

    // make an reference to the node, make sure it not be delected
    ComponentFactory::instance().nodes.push_back(node);

    return node;
}

EntityPtr CreateNodeEntity()
{
    auto entity = CreateEntity();
    AddComponent<Node, Transform>(entity);

    entity->switchState(EntityState::Awaked);

    return entity;
}

EntityPtr CreateNodeEntity(const string &name)
{
    auto node = CreateNodeEntity();
    node->setName(name);

    return node;
}

EntityPtr CreateMeshEntity()
{
    auto entity = CreateEntity();

    AddComponent<Node, Transform, RenderElement>(entity);
    entity->switchState(EntityState::Awaked);

    entity->getComponent<RenderElement>()->getMaterial()->initDefaultPass();

    return entity;
}

EntityPtr CreateSkinningMeshEntity()
{
    auto entity = CreateEntity();

    AddComponent<Node, Transform, Skeleton, SkeletonController, Animation, RenderElement>(entity);
    entity->switchState(EntityState::Awaked);

    entity->getComponent<RenderElement>()->getMaterial()->initDefaultPass();

    return entity;
}

EntityPtr CreateBoneEntity()
{
    auto entity = CreateEntity();

    AddComponent<Node, Transform, Bone>(entity);
    entity->switchState(EntityState::Awaked);

    return entity;
}

template <>
ComponentHandle<ui::Widget> CreateNode2D<ui::Widget>()
{
    auto entity = CreateEntity();

    AddComponent<Node, ui::Transform2D, ui::HierarchyColor, ui::LayoutElement, ui::Widget>(entity);

    entity->switchState(EntityState::Awaked);

    auto component = entity->getComponent<ui::Widget>();

    return component;
}

} // namespace re

