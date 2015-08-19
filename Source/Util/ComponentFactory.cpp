#include "ComponentFactory.h"

#include "UI/Base/HierarchyColor.h"
#include "UI/Base/Transform2D.h"
#include "UI/Base/Label.h"
#include "UI/Base/Sprite.h"
#include "UI/Base/NinePatch.h"

#include "Scene/RenderElement.h"
#include "Scene/Mesh.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/SkeletonController.h"
#include "Animation/Bone.h"
#include "Animation/AnimationTrack.h"

namespace re {

EntityPtr CreateNode()
{
    auto node = std::make_shared<Entity>();

    ComponentFactory::getInstance().nodes.push_back(node);

    return node;
}

EntityPtr CreateNode(const string &name)
{
    auto node = CreateNode();
    node->setName(name);

    return node;
}

EntityPtr CreateMeshNode()
{
    auto node = CreateNode();

    AddComponent<1, RenderElement>::Do(node);

    node->getComponent<RenderElement>()->getMaterial()->initDefaultPass();

    return node;
}

EntityPtr CreateSkinningMesh()
{
    auto node = CreateNode();

    AddComponent<4, Skeleton, SkeletonController, Animation, RenderElement>::Do(node);

    node->getComponent<RenderElement>()->getMaterial()->initDefaultPass();

    return node;
}

EntityPtr CreateBoneNode()
{
    auto node = CreateNode();

    AddComponent<1, Bone>::Do(node);

    return node;
}

} // namespace re

