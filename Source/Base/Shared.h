#ifndef SHARED_H
#define SHARED_H

#include <memory>

namespace re {

template<class T>
using SharedPtr = std::shared_ptr<T>;

template<class T, typename... Args>
SharedPtr<T> Create(Args... args) {
    auto obj = std::make_shared<T>(args...);

    return obj;
}

template<typename T, typename... Args>
class Shared
{
public:
    typedef SharedPtr<T> ptr;
    typedef SharedPtr<const T> constPtr;

public:
    static SharedPtr<T> create(Args... args)
    {
        return std::make_shared<T>(args...);
    }
};

}

#endif // SHARED_H



