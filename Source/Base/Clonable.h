#ifndef CLONABLE_H
#define CLONABLE_H

#include <memory>
#include "Uncopyable.h"

namespace re {

template<class T>
SharedPtr<T> CreateCloneInstance() {
    return std::make_shared<T>();
};

template<class T>
class Clonable : public Uncopyable {
public:
    virtual ~Clonable() {};

    virtual SharedPtr<T> clone() const = 0;
};

}
#endif // CLONABLE_H
