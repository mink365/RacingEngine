#include "Uniform.h"

namespace re {

Uniform::Uniform()
{
    this->init();
}

Uniform::Uniform(const std::string name)
{
    this->init();

    this->name = name;
}

void Uniform::init()
{
    this->location = -1;
    this->type = UNIFORM_FLOAT;
    this->nElements = 0;
    this->data = NULL;
}

float *Uniform::getData() const
{
    return data;
}

void Uniform::setData(float *value)
{
    data = value;
}

}
