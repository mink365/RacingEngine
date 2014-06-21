/*
 * Vector.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Math/Vector.h"
#include "Quaternion.h"

namespace re {

void Vec2::lerp(const Vec2 &v1, const Vec2 &v2, const float l)
{
    if ( l <= 0.0f ) {
        (*this) = v1;
    } else if ( l >= 1.0f ) {
        (*this) = v2;
    } else {
        (*this) = v1 + l * ( v2 - v1 );
    }
}

Vec3::Vec3()
    : x(0), y(0), z(0)
{

}

Vec3::Vec3(const float x, const float y, const float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3 &Vec3::set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;

    return *this;
}

Vec3 &Vec3::set(const Vec3 &v)
{
    this->set(v.x, v.y, v.z);

    return *this;
}

Quat Vec3::toQuat() const
{
    Quat result;

    result.fromAngles(*this);

    return result;
}

Vec4::Vec4() {

}

Vec4::Vec4(const float x, const float y, const float z, const float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

}
