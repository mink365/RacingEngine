#ifndef EVENTUTIL
#define EVENTUTIL

#include <map>
#include <unordered_map>
#include "Message/Signal.h"

using namespace re;

namespace re {
    class BaseComponent;
}

enum class Events
{
    Awake,
    Start,
    Update,
    Enable,
    Disable,
    Enter,
    Exit,
    Destroy,
};

class EventFactory
{
public:
    static void RegisterEvents();

    using GetSignalFunc = std::function<Signal<void()>&(BaseComponent&)>;
    using EventConnection = Connection<void(), CollectorDefault<void>>;

    static std::map<Events, GetSignalFunc> eventSignals;
    // because of the inherit of component, may more than on connection
    static std::unordered_map<BaseComponent*, std::map<Events, std::vector<EventConnection>>> connections;
};

template<class Class, class R, class... Args>
void RegisterEvent(Events event, Class *object, R (Class::*method) (Args...))
{
    if (EventFactory::eventSignals.count(event) == 0) {
        return;
    }

    Signal<void()>& signal = EventFactory::eventSignals[event](*object);

    // TODO: can't write like this
//    signal.connect(slot(object, &Class::method));
    auto func = [object, method] (Args... args) { return (object ->* method) (args...); };
    auto _slot = Slot<R (Args...)>(func);
    auto connection = signal.connect(_slot);

    // save the connection
    EventFactory::connections[object][event].push_back(connection);
}

void CallEvent(BaseComponent* object, Events event);

#endif // EVENTUTIL
