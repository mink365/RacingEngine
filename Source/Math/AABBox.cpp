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

Vec3 AABBox::Corner(uint32_t index) const
{
    return Vec3((index & 1UL) ? this->max.x : this->min.x,
                (index & 2UL) ? this->max.y : this->min.y,
                (index & 4UL) ? this->max.z : this->min.z);
}

std::ostream &operator<<(std::ostream &os, const AABBox &box)
{
    return os << fmt::format("AABB({}, {})", box.min, box.max);
}

} // namespace re
