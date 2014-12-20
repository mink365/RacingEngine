/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_RAY_H
#define RE_RAY_H

#include "Vector.h"

namespace re {

class Ray
{
public:
    Ray();
    ~Ray();

    const Vec3& Origin() const;
    const Vec3& Direction() const;

private:
    Vec3 origin;
    Vec3 direction;
};

inline const Vec3& Ray::Origin() const
{
    return origin;
}

inline const Vec3& Ray::Direction() const
{
    return direction;
}

} // namespace re

#endif // RE_RAY_H
