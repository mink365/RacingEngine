#include "SpotLight.h"

namespace re {

SpotLightData::SpotLightData()
{

}

float SpotLightData::getSpotAngle() const
{
    return spotAngle;
}

void SpotLightData::setSpotAngle(float value)
{
    spotAngle = value;
}

float SpotLightData::getIntensity() const
{
    return intensity;
}

void SpotLightData::setIntensity(float value)
{
    intensity = value;
}

} // namespace re
