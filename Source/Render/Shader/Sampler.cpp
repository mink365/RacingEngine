#include "Sampler.h"

namespace re {

Sampler::Sampler(const char *name, int location, int textureUnit)
{
    this->name = name;
    this->location = location;
    this->unit = textureUnit;
    this->nElements = 1;
}

}
