/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "Plane.h"

namespace re {

Plane::Plane(float a, float b, float c, float d)
{
    this->normal = {a, b, c};
    this->d_ = d;
}

Plane Plane::normalize() const
{
    float inv(1 / this->Normal().length());

    return Plane(a() * inv, b() * inv, c() * inv, d() * inv);
}

} // namespace re

