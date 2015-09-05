#ifndef RE_ENTITYMANAGER_H
#define RE_ENTITYMANAGER_H

#include <vector>
#include "Base/Singleton.h"
#include "Base/Shared.h"
#include "ComponentHandle.h"

namespace re {

class BaseComponent;
class Entity;
using EntityPtr = SharedPtr<Entity>;

class EntityManager : public Singleton<EntityManager>
{
public:
    EntityManager();

    void CacheComponent(SharedPtr<BaseComponent> comp)
    {
        this->components.push_back(comp);
    }

public:
    std::vector<EntityPtr> nodes;
    std::vector<SharedPtr<BaseComponent>> components;
};

template <class T, typename... Args>
ComponentHandle<T> CreateComponent(Args... args)
{
    auto p = Create<T>(args...);

    EntityManager::instance().CacheComponent(p);

    ComponentHandle<T> handle;
    handle.ptr = p;

    return handle;
}

} // namespace re

#endif // RE_ENTITYMANAGER_H
