#include "ComponentFactory.h"

#include "UI/Base/HierarchyColor.h"
#include "UI/Base/Transform2D.h"
#include "UI/Base/Label.h"
#include "UI/Base/Sprite.h"
#include "UI/Base/NinePatch.h"

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

    return node;
}

NodePtr CreateMeshNode()
{
    auto node = Create<Node>();

    AddComponent<2, Mesh, Material>::Do(node);

    return node;
}

NodePtr CreateSkinningMesh()
{
    auto node = Create<Node>();

    AddComponent<5, Skeleton, SkeletonController, Animation, Mesh, Material>::Do(node);

    return node;
}

NodePtr CreateBoneNode()
{
    auto node = Create<Node>();

    AddComponent<1, Bone>::Do(node);

    return node;
}

} // namespace re

