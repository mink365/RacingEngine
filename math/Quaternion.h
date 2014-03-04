 /*
 * Quaternion.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include<cmath>

class reMat3;
class reMat4;
class reVec3;

class reQuat {
public:
    float x;
    float y;
    float z;
    float w;

    reQuat();
    reQuat(float x, float y, float z, float w);

    void set(float x, float y, float z, float w);

    float operator [](int index) const;
    float &operator [](int index);

    reQuat inverse() const;
    float  getLength() const;
    reQuat &normalize();

    reVec3 toVec3() const;
    reMat3 toMat3() const;
    reMat4 toMat4() const;

    reQuat &fromAngles(const reVec3 &angles);
    reQuat &fromAxes(const reVec3 &xAxis, const reVec3 &yAxis, const reVec3 &zAxis);
    reQuat &fromRotationMatrix(float m00, float m01, float m02,
                               float m10, float m11, float m12,
                               float m20, float m21, float m22);

    reQuat &slerp(reQuat &q2, float changeAmnt);
};

inline float reQuat::operator [](int index) const
{
    return (&x)[index];
}

inline float &reQuat::operator [](int index)
{
    return (&x)[index];
}

inline reQuat reQuat::inverse() const
{
    return reQuat(-x, -y, -z, w);
}

inline float reQuat::getLength() const
{
    float len;

    len = x * x + y * y + z * z + w * w;
    return std::sqrt(len);
}

inline reQuat &reQuat::normalize()
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

#endif /* QUATERNION_H_ */
