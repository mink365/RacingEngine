 /*
 * Quaternion.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include<cmath>

namespace re {

class Mat3;
class Mat4;
class Vec3;

class Quat {
public:
    float x;
    float y;
    float z;
    float w;

    Quat();
    Quat(float x, float y, float z, float w);

    void set(float x, float y, float z, float w);

    float operator [](int index) const;
    float &operator [](int index);

    Quat inverse() const;
    float  getLength() const;
    Quat &normalize();

    Vec3 toVec3() const;
    Mat3 toMat3() const;
    Mat4 toMat4() const;

    Quat &fromAngles(const Vec3 &angles);
    Quat &fromAxes(const Vec3 &xAxis, const Vec3 &yAxis, const Vec3 &zAxis);
    Quat &fromRotationMatrix(float m00, float m01, float m02,
                               float m10, float m11, float m12,
                               float m20, float m21, float m22);

    Quat &slerp(Quat &q2, float changeAmnt);
};

inline float Quat::operator [](int index) const
{
    return (&x)[index];
}

inline float &Quat::operator [](int index)
{
    return (&x)[index];
}

inline Quat Quat::inverse() const
{
    return Quat(-x, -y, -z, w);
}

inline float Quat::getLength() const
{
    float len;

    len = x * x + y * y + z * z + w * w;
    return std::sqrt(len);
}

inline Quat &Quat::normalize()
{
    float len;
    float ilength;

    len = this->getLength();

    if (len) {
        ilength = 1 / len;
        x *= ilength;
        y *= ilength;
        z *= ilength;
        w *= ilength;
    }

    return *this;
}

} // namespace re

#endif /* QUATERNION_H_ */
