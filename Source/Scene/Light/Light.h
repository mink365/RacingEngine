#ifndef LIGHT_H
#define LIGHT_H

#include "Scene/Component.h"
#include "Math/Color.h"

namespace re {

enum class LightType {
    Directional,
    Point,
    Spot,
    Area,
};

class ShadowInfo {
public:
    float shadowCameraNear;
    float shadowCameraFar;

    float shadowBias;
    float shadowDarkness;

    std::shared_ptr<RenderTarget> renderTarget;
};

class Light : public Component
{
    friend class RenderView;

public:
    Light();
    virtual ~Light();

    LightType getType() const;

    Color getColor() const;
    void setColor(const Color &value);

    bool getCastShadow() const;
    void setCastShadow(bool value);

    bool getOnlyShadow() const;
    void setOnlyShadow(bool value);

protected:
    LightType type;

    Color color;

    bool castShadow;
    bool onlyShadow;
};

} // namespace re

#endif // LIGHT_H
