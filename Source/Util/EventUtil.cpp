#include "EventUtil.h"
#include "GameHub.h"
#include "Scene/Entity.h"
#include "Scene/Component.h"

std::map<Events, EventFactory::GetSignalFunc> EventFactory::eventSignals;
std::unordered_map<Component*, std::map<Events, std::vector<EventFactory::EventConnection>>> EventFactory::connections;

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

    func = [](re::Component& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->enableEvent;
    };
    eventSignals[Events::Enable] = func;

    func = [](re::Component& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->disableEvent;
    };
    eventSignals[Events::Disable] = func;

    func = [](re::Component& comp) -> re::Signal<void()>&
    {
        return comp.getEntity()->destroyEvent;
    };
    eventSignals[Events::Destroy] = func;
}

void CallEvent(Component* object, Events event)
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
