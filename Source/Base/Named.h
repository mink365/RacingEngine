#ifndef NAMED_H
#define NAMED_H

#include <memory>
#include "platform.h"
#include "Uncopyable.h"

namespace re {

class Named {
public:
    Long getId() const;

    const std::string& getName() const;
    void setName(const std::string& name);

    Long getTag() const;
    void setTag(const Long &value);

protected:
    Long id;
    std::string name;

    Long tag;
};

inline Long Named::getId() const
{
    return this->id;
}

inline const std::string& Named::getName() const
{
    return name;
}

inline void Named::setName(const std::string &name)
{
    this->name = name;
}

inline Long Named::getTag() const
{
    return tag;
}

inline void Named::setTag(const Long &value)
{
    tag = value;
}


} // end namespace

#endif // NAMED_H
