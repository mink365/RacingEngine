#include "ComponentFactory.h"

#include "Base/ECS/EntityManager.h"
#include "UI/Base/HierarchyColor.h"
#include "UI/Base/Transform2D.h"
#include "UI/Layout/LayoutElement.h"
#include "UI/Widget.h"
#include "UI/Base/Text.h"
#include "UI/Base/Image.h"

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
    EntityManager::instance().nodes.push_back(node);

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

    AddComponent<Node, Transform, SkeletonController, RenderElement>(entity);
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

} // namespace re

