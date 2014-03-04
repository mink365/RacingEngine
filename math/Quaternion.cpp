/*
 * Quaternion.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "math/Quaternion.h"
#include <math/Vector.h>
#include <math/Matrix.h>
#include <math/Matrix.h>

reQuat::reQuat() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 1;
}

reQuat::reQuat(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void reQuat::set(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

reMat3 reQuat::toMat3() const
{
    reMat3	mat;
    float	wx, wy, wz;
    float	xx, yy, yz;
    float	xy, xz, zz;
    float	x2, y2, z2;

    x2 = x + x;
    y2 = y + y;
    z2 = z + z;

    xx = x * x2;
    xy = x * y2;
    xz = x * z2;

    yy = y * y2;
    yz = y * z2;
    zz = z * z2;

    wx = w * x2;
    wy = w * y2;
    wz = w * z2;

    // TODO: 交换顺序？
    mat[ 0 ][ 0 ] = 1.0f - (yy + zz);
    mat[ 0 ][ 1 ] = xy - wz;
    mat[ 0 ][ 2 ] = xz + wy;

    mat[ 1 ][ 0 ] = xy + wz;
    mat[ 1 ][ 1 ] = 1.0f - (xx + zz);
    mat[ 1 ][ 2 ] = yz - wx;

    mat[ 2 ][ 0 ] = xz - wy;
    mat[ 2 ][ 1 ] = yz + wx;
    mat[ 2 ][ 2 ] = 1.0f - (xx + yy);

    return mat;
}

reMat4 reQuat::toMat4() const
{
    return this->toMat3().toMat4();
}

reVec3 reQuat::toVec3() const
{
    reVec3 result;

    float sqx = x * x;
    float sqy = y * y;
    float sqz = z * z;

    result.x = std::atan2(2 * (y * z + w * x), 1 - 2 * (sqx + sqy));
    result.y = std::asin(2 * (w * y - z * x));
    result.z = std::atan2(2 * (x * y + w * z), 1 - 2 * (sqy + sqz));

    return result;
}

/**
 * @brief reQuat::fromAngles
 * @param angles
 * @return
 *
 * 由Euler angles 换算出四元数
 * http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
 */
reQuat &reQuat::fromAngles(const reVec3 &angles)
{
    float angle;
    float sinRoll, sinPitch, sinYaw, cosRoll, cosPitch, cosYaw;

    angle = angles.y * 0.5f;
    sinPitch = std::sin(angle);
    cosPitch = std::cos(angle);
    angle = angles.x * 0.5f;
    sinRoll = std::sin(angle);
    cosRoll = std::cos(angle);
    angle = angles.z * 0.5f;
    sinYaw = std::sin(angle);
    cosYaw = std::cos(angle);

    w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
    x = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
    y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
    z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;

    this->normalize();

    return *this;
}

reQuat &reQuat::fromAxes(const reVec3 &xAxis, const reVec3 &yAxis, const reVec3 &zAxis)
{
    this->fromRotationMatrix(xAxis.x, yAxis.x, zAxis.x,
                             xAxis.y, yAxis.y, zAxis.y,
                             xAxis.z, yAxis.z, zAxis.z);
}

reQuat &reQuat::fromRotationMatrix(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
    // Use the Graphics Gems code, from
    // ftp://ftp.cis.upenn.edu/pub/graphics/shoemake/quatut.ps.Z
    // *NOT* the "Matrix and Quaternions FAQ", which has errors!

    // the trace is the sum of the diagonal elements; see
    // http://mathworld.wolfram.com/MatrixTrace.html
    float t = m00 + m11 + m22;

    // we protect the division by s by ensuring that s>=1
    if (t >= 0) { // |w| >= .5
        float s = sqrt(t + 1); // |s|>=1 ...
        w = 0.5f * s;
        s = 0.5f / s;                 // so this division isn't bad
        x = (m21 - m12) * s;
        y = (m02 - m20) * s;
        z = (m10 - m01) * s;
    } else if ((m00 > m11) && (m00 > m22)) {
        float s = sqrt(1.0f + m00 - m11 - m22); // |s|>=1
        x = s * 0.5f; // |x| >= .5
        s = 0.5f / s;
        y = (m10 + m01) * s;
        z = (m02 + m20) * s;
        w = (m21 - m12) * s;
    } else if (m11 > m22) {
        float s = sqrt(1.0f + m11 - m00 - m22); // |s|>=1
        y = s * 0.5f; // |y| >= .5
        s = 0.5f / s;
        x = (m10 + m01) * s;
        z = (m21 + m12) * s;
        w = (m02 - m20) * s;
    } else {
        float s = sqrt(1.0f + m22 - m00 - m11); // |s|>=1
        z = s * 0.5f; // |z| >= .5
        s = 0.5f / s;
        x = (m02 + m20) * s;
        y = (m21 + m12) * s;
        w = (m10 - m01) * s;
    }

    return *this;
}

reQuat &reQuat::slerp(reQuat &q2, float changeAmnt)
{
    if (this->x == q2.x && this->y == q2.y && this->z == q2.z
            && this->w == q2.w) {
        return *this;
    }

    float result = (this->x * q2.x) + (this->y * q2.y) + (this->z * q2.z)
            + (this->w * q2.w);

    if (result < 0.0f) {
        // Negate the second quaternion and the result of the dot product
        q2.x = -q2.x;
        q2.y = -q2.y;
        q2.z = -q2.z;
        q2.w = -q2.w;
        result = -result;
    }

    // Set the first and second scale for the interpolation
    float scale0 = 1 - changeAmnt;
    float scale1 = changeAmnt;

    // Check if the angle between the 2 quaternions was big enough to
    // warrant such calculations
    if ((1 - result) > 0.1f) {
        // Get the angle between the 2 quaternions, and then store the sin()
        // of that angle
        float theta = std::acos(result);
        float invSinTheta = 1.0f / std::sin(theta);

        // Calculate the scale for q1 and q2, according to the angle and
        // it's sine value
        scale0 = std::sin((1.0f - changeAmnt) * theta) * invSinTheta;
        scale1 = std::sin((changeAmnt * theta)) * invSinTheta;
    }

    // Calculate the x, y, z and w values for the quaternion by using a
    // special
    // form of linear interpolation for quaternions.
    this->x = (scale0 * this->x) + (scale1 * q2.x);
    this->y = (scale0 * this->y) + (scale1 * q2.y);
    this->z = (scale0 * this->z) + (scale1 * q2.z);
    this->w = (scale0 * this->w) + (scale1 * q2.w);

    return *this;
}
