/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_AABBOX_H
#define RE_AABBOX_H

#include "Vector.h"

namespace re {

class AABBox
{
public:
    AABBox() {}
    AABBox(const Vec3& min, const Vec3 max);

    Vec3 Center() const;
    Vec3 HalfSize() const;

    const Vec3& Min() const;
    const Vec3& Max() const;

    Vec3 Corner(uint32_t index) const;

    friend std::ostream &operator<<(std::ostream& os, const AABBox& box);

private:
    Vec3 min, max;
};

inline const Vec3& AABBox::Min() const
{
    return min;
}

inline const Vec3& AABBox::Max() const
{
    return max;
}

} // namespace re

#endif // RE_AABBOX_H
