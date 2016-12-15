/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_OBBOX_H
#define RE_OBBOX_H

#include "Vector.h"
#include "Quaternion.h"

namespace re {

class OBBox
{
public:
    OBBox() {}
    OBBox(const Vec3& center, const Quat& rotation, const Vec3& extent_);

    const Vec3& Center() const;
    const Quat& Rotation() const;
    const Vec3& HalfSize() const;

    Vec3 Axis(uint32_t axis) const;

    Vec3 Corner(uint32_t index) const;

    friend std::ostream &operator<<(std::ostream &os, const OBBox& obb);

private:
    Vec3 center_;
    Quat rotation_;

    Vec3 extent_;
};

inline const Vec3& OBBox::Center() const
{
    return center_;
}

inline const Quat& OBBox::Rotation() const
{
    return rotation_;
}

inline const Vec3& OBBox::HalfSize() const {
    return extent_;
}

} // namespace re

#endif // RE_OBBOX_H
