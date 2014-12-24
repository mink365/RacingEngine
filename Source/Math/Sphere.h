/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_SPHERE_H
#define RE_SPHERE_H

#include "Vector.h"

namespace re {

class Sphere
{
public:
    Sphere(const Vec3& center, float radius);

    Vec3 Center();
    const Vec3& Center() const;
    float& Radius();
    float Radius() const;

private:
    Vec3 center;
    float radius;
};

} // namespace re

#endif // RE_SPHERE_H
