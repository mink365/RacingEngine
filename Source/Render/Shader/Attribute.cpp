#include "Attribute.h"

namespace re {

Attribute::Attribute()
{
    this->init();
}

Attribute::Attribute(const std::string name)
{
    this->init();

    this->name = name;
}

void Attribute::setValue(int location, int size, AttributeFormat type, int stribe, int offset)
{
    this->location = location;
    this->size = size;
    this->type = type;
    this->stride = stribe;
    this->offset = offset;
}

void Attribute::init()
{
    this->location = -1;
    this->size = 0;
    this->normalized = false;
    this->type = AttributeFormat::FLOAT;
    this->stride = 0;
    this->offset = 0;
}

int Attribute::getStride() const
{
    return stride;
}

void Attribute::setStride(int value)
{
    stride = value;
}

AttributeFormat Attribute::getType() const
{
    return type;
}

void Attribute::setType(const AttributeFormat value)
{
    type = value;
}

int Attribute::getOffset() const
{
    return offset;
}

void Attribute::setOffset(int value)
{
    offset = value;
}

int Attribute::getSize() const
{
    return size;
}

void Attribute::setSize(int value)
{
    size = value;
}

int Attribute::getLocation() const
{
    return location;
}


bool Attribute::getNormalized() const
{
    return normalized;
}

void Attribute::setNormalized(bool value)
{
    normalized = value;
}

}
