#ifndef SINGLETON_H
#define SINGLETON_H

#include "Uncopyable.h"

namespace re {

template <class T>
class Singleton : public Uncopyable
{
public:
    static T& instance() {
        static T _instance;
        return _instance;
    }
protected:
    Singleton() {}
    ~Singleton() {}
};

}

#endif // SINGLETON_H
