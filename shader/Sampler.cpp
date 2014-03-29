#include "Sampler.h"

namespace re {

Sampler::Sampler(const char *name, uint index)
{
    this->name = name;
    this->location = index;
}

}
