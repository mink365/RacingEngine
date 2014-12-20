#ifndef SHARED_H
#define SHARED_H

#include <memory>

namespace re {

template<class T, typename... Args>
std::shared_ptr<T> Create(Args... args) {
    auto obj = std::make_shared<T>();

    if (obj != nullptr) {
        obj->init(args...);
    }

    return obj;
}

template<class T, typename... Args>
class Shared
{
public:
    typedef std::shared_ptr<T> ptr;
    typedef std::shared_ptr<const T> constPtr;

public:
    static std::shared_ptr<T> create(Args... args);
};

template<class T, typename... Args>
std::shared_ptr<T> Shared<T, Args...>::create(Args... args)
{
    return std::make_shared<T>(args...);
}

}

#endif // SHARED_H



