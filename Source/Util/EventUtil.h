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

    static std::map<Events, GetSignalFunc> eventSignals;
};

template<class Class, class R, class... Args>
void RegisterEvent(Events event, Class *object, R (Class::*method) (Args...))
{
    if (EventFactory::eventSignals.count(event) == 0) {
        return;
    }

    Signal<void()>& signal = EventFactory::eventSignals[event](*object);

//    signal.connect(slot(object, &Class::method));
    auto func = [object, method] (Args... args) { return (object ->* method) (args...); };
    auto _slot = Slot<R (Args...)>(func);
    signal.connect(_slot);
}

#endif // EVENTUTIL
