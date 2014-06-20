#ifndef SINGLETON_H
#define SINGLETON_H

#include "Uncopyable.h"

namespace re {

template <class T>
class Singleton : public Uncopyable
{
public:
    static T& getInstance() {
        static T instance;
        return instance;
    }
protected:
    Singleton() {};
    ~Singleton() {};
};

}

#endif // SINGLETON_H
