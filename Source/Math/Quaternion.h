 /*
 * Quaternion.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include<cmath>

#include "platform.h"

namespace re {

class Mat3;
class Mat4;
class Vec3;

class Quat {
public:
    static Quat Identity();

public:
    Quat();
    Quat(float x, float y, float z, float w);

    void set(float x, float y, float z, float w);

    float operator [](int index) const;
    float &operator [](int index);

    Quat operator *(const float a) const;
    Quat operator /(const float a) const;

    Quat &operator *=(const float a);
    Quat &operator /=(const float a);

    Quat            inverse() const;
    float           length() const;
    float           lengthSqr() const;
    Quat            normalize() const;
    Quat&           normalizeSelf();

    float *toFloatPtr();
    const float *toFloatPtr () const;

    Vec3 toVec3() const;
    Mat3 toMat3() const;
    Mat4 toMat4() const;

    Quat &fromAngles(const Vec3 &angles);
    Quat &fromAxes(const Vec3 &xAxis, const Vec3 &yAxis, const Vec3 &zAxis);
    Quat &fromRotationMatrix(float m00, float m01, float m02,
                               float m10, float m11, float m12,
                               float m20, float m21, float m22);

    Quat &slerp(Quat &q2, float changeAmnt);

    std::string toString() const;

public:
    float x;
    float y;
    float z;
    float w;
};

inline float Quat::operator [](int index) const
{
    return (&x)[index];
}

inline float &Quat::operator [](int index)
{
    return (&x)[index];
}

inline Quat Quat::operator *(const float a) const
{
    return Quat(x * a, y * a, z * a, w * a);
}

inline Quat Quat::operator /(const float a) const
{
    float inva = 1.0f / a;

    return Quat(x * inva, y * inva, z * inva, w * inva);
}

inline Quat &Quat::operator *=(const float a)
{
    this->x *= a;
    this->y *= a;
    this->z *= a;
    this->w *= a;

    return *this;
}

inline Quat &Quat::operator /=(const float a)
{
    float inva = 1.0f / a;

    this->x *= inva;
    this->y *= inva;
    this->z *= inva;
    this->w *= inva;

    return *this;
}

inline Quat Quat::inverse() const
{
    return Quat(-x, -y, -z, w);
}

inline float Quat::length() const
{
    float len;

    len = x * x + y * y + z * z + w * w;
    return std::sqrt(len);
}

inline float Quat::lengthSqr() const
{
    return (x * x + y * y + z * z + w * w);
}

inline Quat Quat::normalize() const
{
    return *this / length();
}

inline Quat &Quat::normalizeSelf()
{
    float len;
    float ilength;

    len = this->length();

    if (len) {
        ilength = 1 / len;
        x *= ilength;
        y *= ilength;
        z *= ilength;
        w *= ilength;
    }

    return *this;
}

inline std::string Quat::toString() const
{
    return StringUtil::Printf("Quat(%f, %f, %f, %f)", x, y, z, w);
}

} // namespace re

#endif /* QUATERNION_H_ */
