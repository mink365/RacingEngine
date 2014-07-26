#ifndef RE_SPOTLIGHT_H
#define RE_SPOTLIGHT_H

#include "Light.h"

namespace re {

class SpotLight : public Light
{
    friend class RenderView;

public:
    SpotLight();

    float getSpotAngle() const;
    void setSpotAngle(float value);

    float getIntensity() const;
    void setIntensity(float value);

protected:
    float spotAngle;
    float intensity;

    ShadowInfo shadow;

    // TODO: use spotAngle ?
    float shadowCameraFov;
};

} // namespace re

#endif // RE_SPOTLIGHT_H
