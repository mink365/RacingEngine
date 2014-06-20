#ifndef SHARED_H
#define SHARED_H

#include <memory>

namespace re {

template<class T>
class Shared
{
public:
    typedef std::shared_ptr<T> type;
    typedef std::shared_ptr<const T> constType;

public:
    static std::shared_ptr<T> create();

//    static std::shared_ptr<T> create(Args... args) {
////        return std::make_shared<T>(std::forward(args)...);

//        T* t = new T(std::forward<args>...);

//        return nullptr;
//    }
};

template<class T>
std::shared_ptr<T> Shared<T>::create()
{
//    T* temp = new T(std::forward<args>...);

    return std::make_shared<T>();
}

//template<class T, typename... Args>
//std::shared_ptr<T> Shared<T, ...Args>::create(Args... args)
//{
//    return std::make_shared<T>();
//}

}

#endif // SHARED_H



