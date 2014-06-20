#ifndef CLONABLE_H
#define CLONABLE_H

#include <memory>
#include "Uncopyable.h"

namespace re {

template<class T>
class Clonable : public Uncopyable {
public:
    virtual ~Clonable() {};

    virtual std::shared_ptr<T> clone() const = 0;
};

}
#endif // CLONABLE_H
