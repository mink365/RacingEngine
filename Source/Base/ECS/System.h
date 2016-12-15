#ifndef RE_SYSTEM_H
#define RE_SYSTEM_H

#include <typeindex>
#include <typeinfo>
#include <assert.h>
#include <unordered_map>
#include "Base/Singleton.h"

namespace re {

class EventManager;

class System : public Uncopyable
{
public:
    System();

public:
    virtual void configure(EventManager &events) {}

    virtual void update() {}
};

class SystemManager : public Singleton<SystemManager>
{
public:
    SystemManager();

    template<typename S>
    void add(std::shared_ptr<S> system)
    {
        auto id = std::type_index(typeid(S));
        systems_[id] = system;
    }

    template<typename S, typename... Args>
    std::shared_ptr<S> add(Args... args)
    {
        auto system = std::make_shared<S>(args...);

        auto id = std::type_index(typeid(S));
        systems_[id] = system;

        return system;
    }

    template<typename S>
    std::shared_ptr<S> system()
    {
        auto id = std::type_index(typeid(S));

        auto it = systems_.find(id);
        assert(it != systems_.end());
        return it == systems_.end()
            ? nullptr
            : std::shared_ptr<S>(std::static_pointer_cast<S>(it->second));
    }

    void configure();

private:
    EventManager& eventManager_;

    std::unordered_map<std::type_index, std::shared_ptr<System>> systems_;
};

} // namespace re

#endif // RE_SYSTEM_H
