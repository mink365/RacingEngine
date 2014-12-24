/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "Frustum.h"

namespace re {

Frustum::Frustum()
{

}

void Frustum::FrustumPlane(uint32_t index, const Plane &plane)
{
    this->planes[index] = plane;
}

void Frustum::Corner(uint32_t index, const Vec3 &v)
{
    this->corners[index] = v;
}

} // namespace re

