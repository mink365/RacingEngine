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

NodePtr CreateNode()
{
    auto node = Create<Node>();

    ComponentFactory::getInstance().nodes.push_back(node);

    return node;
}

NodePtr CreateNode(const string &name)
{
    auto node = CreateNode();
    node->setName(name);

    return node;
}

NodePtr CreateMeshNode()
{
    auto node = CreateNode();

    AddComponent<1, RenderElement>::Do(node);

    node->getComponent<RenderElement>()->getMaterial()->initDefaultPass();

    return node;
}

NodePtr CreateSkinningMesh()
{
    auto node = CreateNode();

    AddComponent<4, Skeleton, SkeletonController, Animation, RenderElement>::Do(node);

    node->getComponent<RenderElement>()->getMaterial()->initDefaultPass();

    return node;
}

NodePtr CreateBoneNode()
{
    auto node = CreateNode();

    AddComponent<1, Bone>::Do(node);

    return node;
}

} // namespace re

