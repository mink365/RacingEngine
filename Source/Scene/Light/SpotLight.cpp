#include "SpotLight.h"

namespace re {

SpotLight::SpotLight()
{
    this->type = LightType::Spot;
}

float SpotLight::getSpotAngle() const
{
    return spotAngle;
}

void SpotLight::setSpotAngle(float value)
{
    spotAngle = value;
}

float SpotLight::getIntensity() const
{
    return intensity;
}

void SpotLight::setIntensity(float value)
{
    intensity = value;
}

} // namespace re
