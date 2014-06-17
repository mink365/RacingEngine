#ifndef NAMED_H
#define NAMED_H

#include <memory>
#include "platform.h"
#include "Uncopyable.h"

namespace re {

template<class T>
class Clonable : public Uncopyable {
public:
    virtual ~Clonable() {};

    virtual std::shared_ptr<T> clone() const = 0;
};

class Named {
public:
    Long getId();
    void setId(const Long& id);

    std::string getName();
    void setName(const std::string& name);

protected:
    Long id;
    std::string name;
};

inline Long Named::getId()
{
    return this->id;
}

inline void Named::setId(const Long &id)
{
    this->id = id;
}

inline string Named::getName()
{
    return name;
}

inline void Named::setName(const string &name)
{
    this->name = name;
}

} // end namespace

#endif // NAMED_H
