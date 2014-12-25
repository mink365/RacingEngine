/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "Ray.h"

namespace re {

Ray::Ray()
{

}

Ray::Ray(const Vec3 &orig, const Vec3 &dir)
{
    this->origin = orig;
    this->direction = dir;
}

} // namespace re

