#include "Light.h"

namespace re {

Light::Light()
{
}

Light::~Light()
{

}

LightType Light::getType() const
{
    return this->type;
}

Color Light::getColor() const
{
    return color;
}

void Light::setColor(const Color &value)
{
    color = value;
}
bool Light::getCastShadow() const
{
    return castShadow;
}

void Light::setCastShadow(bool value)
{
    castShadow = value;
}
bool Light::getOnlyShadow() const
{
    return onlyShadow;
}

void Light::setOnlyShadow(bool value)
{
    onlyShadow = value;
}




}
