/*
 * Vector.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Math/Vector.h"

namespace re {

Vec3::Vec3() {

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

float *Vec3::toFloatPtr()
{
    return &x;
}

const float *Vec3::toFloatPtr() const
{
    return &x;
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
