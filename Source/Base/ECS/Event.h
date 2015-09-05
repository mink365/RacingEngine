#ifndef RE_EVENT_H
#define RE_EVENT_H

#include <memory>
#include <utility>
#include <typeinfo>
#include <typeindex>
#include <assert.h>
#include <unordered_map>
#include "Base/Uncopyable.h"
#include "Message/Signal.h"
#include "Base/Singleton.h"

namespace re {

class BaseEvent
{
public:
    BaseEvent();
};

typedef Signal<void (const void*)> EventSignal;
typedef std::shared_ptr<EventSignal> EventSignalPtr;
typedef std::weak_ptr<EventSignal> EventSignalWeakPtr;
using ConnectionType = Connection<void(const void*), CollectorDefault<void>>;

class BaseReceiver
{
public:
    virtual ~BaseReceiver() = default;

private:
    friend class EventManager;
    std::unordered_map<std::type_index, ConnectionType> connections_;
};

template<typename E>
class Receiver : public BaseReceiver
{

};

class EventManager : public Singleton<EventManager>
{
public:
    template<typename E, typename Receiver>
    void subscribe(Receiver& receiver)
    {
        void (Receiver::*receive)(const E &) = &Receiver::receive;
        auto id = std::type_index(typeid(E));
        auto sig = getHandler<E>();
        auto wrapper = EventCallbackWrapper<E>(std::bind(receive, &receiver, std::placeholders::_1));
        auto connection = sig->connect(wrapper);
        BaseReceiver &base = receiver;
        base.connections_[id] = connection;
    }

    template<typename E, typename Receiver>
    void unsubscribe(Receiver& receiver)
    {
        BaseReceiver &base = receiver;

        auto id = std::type_index(typeid(E));
        assert(base.connections_.find(id) != base.connections_.end());
        auto connection = base.connections_[id];
        connection.disconnect();
        base.connections_.erase(id);
    }

    template<typename E>
    void emit(const E& event)
    {
        EventSignalPtr& sig = getHandler<E>();
        sig->emit(&event);
    }

    template<typename E, typename... Args>
    void emit(Args... args)
    {
        E event = E(std::forward<Args>(args) ...);
        this->emit(event);
    }

private:
    template<typename E>
    EventSignalPtr& getHandler()
    {
        auto id = std::type_index(typeid(E));
        if (handlers_.count(id) == 0)
        {
            handlers_[id] = std::make_shared<EventSignal>();
        }
        return handlers_[id];
    }

    template<typename E>
    struct EventCallbackWrapper
    {
        explicit EventCallbackWrapper(std::function<void(const E&)> callback)
                : callback(callback)
        {
        }
        void operator()(const void* event)
        {
            callback(*(static_cast<const E*>(event)));
        }
        std::function<void(const E&)> callback;
    };

    std::unordered_map<std::type_index, EventSignalPtr> handlers_;
};

} // namespace re

#endif // RE_EVENT_H
