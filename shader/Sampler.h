#ifndef SAMPLE_H
#define SAMPLE_H

#include "platform.h"

class Sampler
{
    friend class ShaderUtil;
public:
    Sampler(const char *name, uint index);

    std::string name;

    uint location;
};

#endif // SAMPLE_H
