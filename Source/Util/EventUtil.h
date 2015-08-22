#ifndef EVENTUTIL
#define EVENTUTIL

#include <map>
#include <unordered_map>
#include "Message/Signal.h"
#include "PreDeclare.h"

using namespace re;

enum class Events
{
    Awake,
    Start,
    Update,
    Enable,
    Disable,
    Enter,
    Exit,
};

class EventFactory
{
public:
    static void RegisterEvents();

    using GetSignalFunc = std::function<Signal<void()>&(Component&)>;
    using EventConnection = Connection<void(), CollectorDefault<void>>;

    static std::map<Events, GetSignalFunc> eventSignals;
    // because of the inherit of component, may more than on connection
    static std::unordered_map<Component*, std::map<Events, std::vector<EventConnection>>> connections;
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

void CallEvent(Component* object, Events event);

#endif // EVENTUTIL
