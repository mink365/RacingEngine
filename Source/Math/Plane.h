/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_PLANE_H
#define RE_PLANE_H

#include "Vector.h"

namespace re {

/**
 * @brief The Plane class
 *
 * a*x + b*y + c*z + d = 0
 */
class Plane
{
public:
    Plane();
    ~Plane();

    float a() const;
    float b() const;
    float c() const;
    float d() const;

    const Vec3& Normal() const;

private:
    Vec3 normal;
    float d_;
};

inline const Vec3& Plane::Normal() const
{
    return normal;
}

inline float Plane::a() const
{
    return normal.x;
}

inline float Plane::b() const
{
    return normal.y;
}

inline float Plane::c() const
{
    return normal.z;
}

inline float Plane::d() const
{
    return d_;
}

} // namespace re

#endif // RE_PLANE_H
