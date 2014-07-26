#ifndef RE_DIRECTIONALLIGHT_H
#define RE_DIRECTIONALLIGHT_H

#include "Light.h"
#include "Math/BoundingVolume.h"

namespace re {

class DirectionalLight : public Light
{
    friend class RenderView;

public:
    DirectionalLight();

protected:
    ShadowInfo shadow;
    Rect orthoRect;
};

} // namespace re

#endif // RE_DIRECTIONALLIGHT_H
