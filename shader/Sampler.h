#ifndef SAMPLE_H
#define SAMPLE_H

#include "platform.h"

namespace re {

class Sampler
{
    friend class ShaderUtil;
public:
    Sampler(const char *name, uint index);

    std::string name;

    uint location;
};

} // namespace re

#endif // SAMPLE_H
