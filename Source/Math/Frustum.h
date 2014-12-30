/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_FRUSTUM_H
#define RE_FRUSTUM_H

#include <array>
#include "Vector.h"
#include "Plane.h"

namespace re {

class Frustum
{
public:
    Frustum();

    const Plane& FrustumPlane(uint32_t index) const;
    const Vec3& Corner(uint32_t index) const;

    void FrustumPlane(uint32_t index, const Plane& plane);
    void Corner(uint32_t index, const Vec3& v);

private:
    std::array<Plane, 6> planes;
    std::array<Vec3, 8> corners;
};

inline const Plane& Frustum::FrustumPlane(uint32_t index) const
{
    return planes[index];
}

inline const Vec3& Frustum::Corner(uint32_t index) const
{
    return corners[index];
}

} // namespace re

#endif // RE_FRUSTUM_H
