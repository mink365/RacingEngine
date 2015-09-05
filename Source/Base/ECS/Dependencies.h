#ifndef RE_DEPENDENCIES_H
#define RE_DEPENDENCIES_H

#include "System.h"
#include "Event.h"
#include "Entity.h"

namespace re {

class StaticCall
{
public:
    StaticCall(std::function<void()> func)
    {
        func();
    }
};

#define COMPONENT_DEPENDENCY(Class, ...) \
static StaticCall __static_##Class([](){ \
    SystemManager::instance().add<Dependency<Class, __VA_ARGS__>>(); \
    SystemManager::instance().configure(); \
});

template<typename C, typename ... Deps>
class Dependency : public System, public Receiver<Dependency<C, Deps...>>
{
public:
    Dependency() = default;

    void receive(const ComponentAddedEvent<C> &event) {
        assign<Deps...>(event.entity);
    }

    void configure(EventManager &events) override {
        events.subscribe<ComponentAddedEvent<C>>(*this);
    }

  private:
    template <typename D>
    void assign(EntityPtr entity) {
        if (!entity->getComponent<D>()) {
            entity->addComponent<D>();
        }
    }

    template <typename D, typename D1, typename ... Ds>
    void assign(EntityPtr entity) {
        assign<D>(entity);
        assign<D1, Ds...>(entity);
    }


};

} // namespace re

#endif // RE_DEPENDENCIES_H
