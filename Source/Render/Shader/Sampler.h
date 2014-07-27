#ifndef SAMPLE_H
#define SAMPLE_H

#include "platform.h"

namespace re {

class Sampler
{
    friend class ShaderUtil;
public:
    Sampler(const char *name, int location, int textureUnit);

    std::string name;

    int location;

    /**
     * @brief nElements
     * for texture array, texture array will only have one
     * name, shadowMap[0], so, we have to save the element size
     * of it.
     */
    int nElements;

    int unit;
};

} // namespace re

#endif // SAMPLE_H
