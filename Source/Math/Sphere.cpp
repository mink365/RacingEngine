/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "Sphere.h"

namespace re {

Sphere::Sphere(const Vec3& center, float radius)
{
    this->center = center;
    this->radius = radius;
}

Vec3 Sphere::Center()
{
    return center;
}

const Vec3 &Sphere::Center() const
{
    return center;
}

float &Sphere::Radius()
{
    return radius;
}

float Sphere::Radius() const
{
    return radius;
}

} // namespace re

