#ifndef LIGHT_H
#define LIGHT_H

#include "Base/ECS/Component.h"
#include "Math/Color.h"
#include "SpotLight.h"
#include "DirectionalLight.h"

namespace re {

class RenderTarget;
using RenderTargetPtr = SharedPtr<RenderTarget>;

enum class LightType {
    Directional = 0,
    Point = 2,
    Spot = 1,
    Area = 3,
};

template<class T>
constexpr int EnumToInt(T type)
{
    return (int)type;
}

class ShadowInfo
{
public:
    float shadowCameraNear;
    float shadowCameraFar;

    float shadowBias;
    float shadowDarkness;

    RenderTargetPtr renderTarget;
};

class Light : public Component<Light>
{
    friend class RenderView;
    friend class RenderManager;

    using LightDataType = std::tuple<DirectionalLightData, SpotLightData>;

public:
    Light();
    virtual ~Light();

    LightType getType() const;
    void setType(LightType type);

    Color getColor() const;
    void setColor(const Color &value);

    bool getCastShadow() const;
    void setCastShadow(bool value);

    bool getOnlyShadow() const;
    void setOnlyShadow(bool value);

    ShadowInfo& getShadowInfo()
    {
        return shadow;
    }

    template<LightType type>
    typename std::tuple_element<EnumToInt(type), LightDataType>::type& getLightData()
    {
        return std::get<EnumToInt(type)>(datas);
    }

protected:
    LightType type;

    Color color;

    bool castShadow;
    bool onlyShadow;

    ShadowInfo shadow;

    LightDataType datas;
};

} // namespace re

#endif // LIGHT_H
