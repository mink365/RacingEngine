#ifndef RE_DIRECTIONALLIGHT_H
#define RE_DIRECTIONALLIGHT_H

#include "Math/Rect.h"

namespace re {

class DirectionalLightData
{
    friend class RenderView;

public:
    DirectionalLightData();

public:
    Rect orthoRect;
};

} // namespace re

#endif // RE_DIRECTIONALLIGHT_H
