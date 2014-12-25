/*
 * Vector.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Math/Vector.h"
#include "Math/Math.h"
#include "Quaternion.h"

namespace re {

const Vec2 Vec2::Zero(0, 0);

std::string Vec2::toString() const
{
    return StringUtil::Printf("Vec2(%f, %f)", x, y);
}

Vec3::Vec3()
    : x(0), y(0), z(0)
{

}

Vec3::Vec3(const float &x, const float &y, const float &z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3 &Vec3::set(const float& x, const float& y, const float& z)
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

std::string Vec3::toString() const
{
    return StringUtil::Printf("Vec3(%f, %f, %f)", x, y, z);
}

Vec4::Vec4()
    :x(0), y(0), z(0), w(0)
{

}

Vec4::Vec4(const float& x, const float& y, const float& z, const float& w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

std::string Vec4::toString() const
{
    return StringUtil::Printf("Vec4(%f, %f, %f, %f)", x, y, z, w);
}

}
