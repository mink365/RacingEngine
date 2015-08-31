#ifndef RE_SPOTLIGHT_H
#define RE_SPOTLIGHT_H

namespace re {

class SpotLightData
{
    friend class RenderView;

public:
    SpotLightData();

    float getSpotAngle() const;
    void setSpotAngle(float value);

    float getIntensity() const;
    void setIntensity(float value);

public:
    float spotAngle;
    float intensity;

    // TODO: use spotAngle ?
    float shadowCameraFov;
};

} // namespace re

#endif // RE_SPOTLIGHT_H
