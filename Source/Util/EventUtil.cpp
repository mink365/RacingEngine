#include "EventUtil.h"
#include "GameHub.h"
#include "Base/ECS/Entity.h"
#include "Base/ECS/Component.h"
#include "Scene/Transform.h"

std::map<Events, EventFactory::GetSignalFunc> EventFactory::eventSignals;
std::unordered_map<BaseComponent*, std::map<Events, std::vector<EventFactory::EventConnection>>> EventFactory::connections;

void EventFactory::RegisterEvents()
{
    EventFactory::GetSignalFunc func;
    func = [](re::BaseComponent& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->enterEvent;
    };
    eventSignals[Events::Enter] = func;

    func = [](re::BaseComponent& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->exitEvent;
    };
    eventSignals[Events::Exit] = func;

    func = [](re::BaseComponent& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->awakeEvent;
    };
    eventSignals[Events::Awake] = func;

    func = [](re::BaseComponent& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->startEvent;
    };
    eventSignals[Events::Start] = func;

    func = [](re::BaseComponent& comp) -> re::Signal<void()>&
    {
        return GameHub::instance().updateEvent;
    };
    eventSignals[Events::Update] = func;

    func = [](re::BaseComponent& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->enableEvent;
    };
    eventSignals[Events::Enable] = func;

    func = [](re::BaseComponent& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->disableEvent;
    };
    eventSignals[Events::Disable] = func;

    func = [](re::BaseComponent& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->destroyEvent;
    };
    eventSignals[Events::Destroy] = func;

    func = [](re::BaseComponent& comp) -> re::Signal<void()>&
    {
        return comp.getComponent<Transform>()->transformRefresh;
    };
    eventSignals[Events::Transform] = func;
}

void CallEvent(BaseComponent *object, Events event)
{
    if (EventFactory::connections.count(object) > 0) {
        if (EventFactory::connections[object].count(event) > 0) {
            auto& connectionList = EventFactory::connections[object][event];

            for (auto& connection : connectionList) {
                auto slot = connection.getSlot();
                if (slot && slot->useable())
                {
                    slot->_func();
                }
            }
        }
    }
}
