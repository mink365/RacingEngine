/*
 * Vector.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "math/Vector.h"


reVec3::reVec3() {

}

reVec3::reVec3(const float x, const float y, const float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

reVec3 &reVec3::set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;

    return *this;
}

reVec3 &reVec3::set(const reVec3 &v)
{
    this->set(v.x, v.y, v.z);

    return *this;
}

float *reVec3::toFloatPtr()
{
    return &x;
}

const float *reVec3::toFloatPtr() const
{
    return &x;
}

reVec4::reVec4() {

}

reVec4::reVec4(const float x, const float y, const float z, const float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}
