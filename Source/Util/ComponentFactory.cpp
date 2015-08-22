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
#include "EventUtil.h"
#include "GameHub.h"
#include <map>

std::map<Events, EventFactory::GetSignalFunc> EventFactory::eventSignals;

void EventFactory::RegisterEvents()
{
    EventFactory::GetSignalFunc func;
    func = [](re::Component& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->enterEvent;
    };
    eventSignals[Events::Enter] = func;

    func = [](re::Component& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->exitEvent;
    };
    eventSignals[Events::Exit] = func;

    func = [](re::Component& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->awakeEvent;
    };
    eventSignals[Events::Awake] = func;

    func = [](re::Component& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->startEvent;
    };
    eventSignals[Events::Start] = func;

    func = [](re::Component& comp) -> re::Signal<void()>&
    {
        return GameHub::getInstance().updateEvent;
    };
    eventSignals[Events::Update] = func;
}

namespace re {

EntityPtr CreateEntity()
{
    auto node = std::make_shared<Entity>();

    // make an reference to the node, make sure it not be delected
    ComponentFactory::getInstance().nodes.push_back(node);

    return node;
}

EntityPtr CreateNode()
{
    auto node = CreateEntity();
    AddComponent<Node, Transform>(node);

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

    AddComponent<RenderElement>(node);

    node->getComponent<RenderElement>()->getMaterial()->initDefaultPass();

    return node;
}

EntityPtr CreateSkinningMesh()
{
    auto node = CreateNode();

    AddComponent<Skeleton, SkeletonController, Animation, RenderElement>(node);

    node->getComponent<RenderElement>()->getMaterial()->initDefaultPass();

    return node;
}

EntityPtr CreateBoneNode()
{
    auto node = CreateNode();

    AddComponent<Bone>(node);

    return node;
}

} // namespace re

