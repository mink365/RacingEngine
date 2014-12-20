/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "AABBox.h"

namespace re {

AABBox::AABBox(const Vec3 &min, const Vec3 max)
{
    this->min = min;
    this->max = max;
}

Vec3 AABBox::Center() const
{
    return (min + max) / 2.0f;
}

Vec3 AABBox::HalfSize() const
{
    return (max - min) / 2.0f;
}

} // namespace re
