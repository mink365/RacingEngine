#ifndef SHARED_H
#define SHARED_H

#include <memory>

namespace re {

template<class T, typename... Args>
std::shared_ptr<T> Create(Args... args) {
    auto obj = std::make_shared<T>(args...);

    return obj;
}

template<typename T, typename... Args>
class Shared
{
public:
    typedef std::shared_ptr<T> ptr;
    typedef std::shared_ptr<const T> constPtr;

public:
    static std::shared_ptr<T> create(Args... args)
    {
        return std::make_shared<T>(args...);
    }
};

}

#endif // SHARED_H



