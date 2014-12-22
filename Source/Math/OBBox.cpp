/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "OBBox.h"
#include "Math.h"

namespace re {

OBBox::OBBox(const Vec3 &center, const Quat &rotation, const Vec3 &extent)
{
    this->center_ = center;
    this->rotation_ = rotation;
    this->extent = extent;
}

Vec3 OBBox::Axis(uint32_t axis) const
{

}

Vec3 OBBox::Corner(uint32_t index) const
{
    Vec3 const & center = this->Center();
    Vec3 const & extent = this->HalfSize();
    Vec3 const extent_x = MathLib::abs(extent.x * this->Axis(0));
    Vec3 const extent_y = MathLib::abs(extent.y * this->Axis(1));
    Vec3 const extent_z = MathLib::abs(extent.z * this->Axis(2));

    return center + ((index & 1UL) ? +extent_x : -extent_x)
        + ((index & 2UL) ? +extent_y : -extent_y)
        + ((index & 4UL) ? +extent_z : -extent_z);
}

} // namespace re
