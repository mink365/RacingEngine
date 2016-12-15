#ifndef COMPONENTHANDLE
#define COMPONENTHANDLE

#include "Base/Shared.h"

namespace re {

class Entity;
class BaseComponent;

template <class C>
class ComponentHandle
{
public:
    template<class T>
    friend class Component;

    template <class T, typename... Args>
    friend ComponentHandle<T> CreateComponent(Args... args);

public:
    typedef C ComponentType;

    ComponentHandle() {}

    ComponentHandle(std::nullptr_t) {}

    ComponentHandle(const ComponentHandle<BaseComponent>& handle) {
        this->entity = handle.entity;
        this->ptr = handle.ptr;
    }

    ComponentHandle(SharedPtr<C> ptr, SharedPtr<Entity> entity) {
        this->entity = entity;
        this->ptr = ptr;
    }

    C *operator -> () {
        return getPtr().get();
    }
    const C *operator -> () const {
        return getPtr().get();
    }

    C *get() {
        return getPtr().get();
    }
    const C *get() const {
        return getPtr().get();
    }

    bool operator == (const ComponentHandle<C> &other) const {
      return this->getPtr() == other.getPtr();
    }

    bool operator != (const ComponentHandle<C> &other) const {
      return !(*this == other);
    }

    bool operator == (std::nullptr_t null) {
        return this->get() == null;
    }

    bool operator != (std::nullptr_t null) {
        return !(this->get() == null);
    }

    operator bool() {
        return this->get() != nullptr;
    }

    operator ComponentHandle<BaseComponent> ()
    {
        return this->getBaseComponentHandle();
    }

    operator SharedPtr<C> ()
    {
        return this->getPtr();
    }

    operator SharedPtr<void> ()
    {
        return this->getPtr();
    }

    SharedPtr<C> getPtr() const {
        return ptr.lock();
    }

    ComponentHandle<BaseComponent> getBaseComponentHandle()
    {
        SharedPtr<BaseComponent> p = std::static_pointer_cast<BaseComponent>(this->ptr.lock());
        ComponentHandle<BaseComponent> handle = ComponentHandle<BaseComponent>(p, this->entity.lock());

        return handle;
    }

protected:
    WeakPtr<C> ptr;

    WeakPtr<Entity> entity;
};

}

#endif // COMPONENTHANDLE

