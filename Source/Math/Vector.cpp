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

Vec2::Vec2(const Vec3 &v)
    : x(v.x), y(v.y)
{

}

Vec2 &Vec2::operator =(const Vec3 &v)
{
    x = v.x;
    y = v.y;

    return *this;
}

std::ostream &operator<<(std::ostream &os, const Vec2 &vec)
{
    return os << fmt::format("Vec2({}, {})", vec.x, vec.y);
}

Vec3::Vec3()
    : x(0), y(0), z(0)
{

}

Vec3::Vec3(const Vec2 &v)
    : x(v.x), y(v.y), z(0)
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

std::ostream &operator<<(std::ostream &os, const Vec3 &vec)
{
    return os << fmt::format("Vec3({}, {}, {})", vec.x, vec.y, vec.z);
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

std::ostream &operator<<(std::ostream &os, const Vec4 &vec)
{
    return os << fmt::format("Vec4({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
}

}
