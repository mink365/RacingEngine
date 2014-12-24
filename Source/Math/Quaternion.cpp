/*
 * Quaternion.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Math/Quaternion.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Matrix.h"

namespace re {

Quat Quat::Identity()
{
    static const Quat out(0, 0, 0, 1);
    return out;
}

Quat::Quat() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 1;
}

Quat::Quat(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Quat::set(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vec3 Quat::v() const
{
    return Vec3(x, y, z);
}

void Quat::v(const Vec3 &rhs)
{
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
}

Quat &Quat::operator *=(const Quat &rhs)
{
    this->x = this->x * rhs.w - this->y * rhs.z + this->z * rhs.y + this->w * rhs.x;
    this->y = this->x * rhs.z + this->y * rhs.w - this->z * rhs.x + this->w * rhs.y;
    this->z = this->y * rhs.x - this->x * rhs.y + this->z * rhs.w + this->w * rhs.z;
    this->w = this->w * rhs.w - this->x * rhs.x - this->y * rhs.y - this->z * rhs.z;

    return *this;
}

Quat Quat::operator *(const Quat &rhs) const
{
    Quat result = *this;

    result *= rhs;

    return result;
}

float *Quat::toFloatPtr()
{
    return &x;
}

const float *Quat::toFloatPtr() const
{
    return &x;
}

Mat3 Quat::toMat3() const
{
    Mat3	mat;
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

Mat4 Quat::toMat4() const
{
    return this->toMat3().toMat4();
}

Vec3 Quat::toVec3() const
{
    Vec3 result;

    float sqx = x * x;
    float sqy = y * y;
    float sqz = z * z;

    result.x = std::atan2(2 * (y * z + w * x), 1 - 2 * (sqx + sqy));
    result.y = std::asin(2 * (w * y - z * x));
    result.z = std::atan2(2 * (x * y + w * z), 1 - 2 * (sqy + sqz));

    return result;
}

/**
 * @brief Quat::fromAngles
 * @param angles
 * @return
 *
 * 由Euler angles 换算出四元数
 * http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
 */
Quat &Quat::fromAngles(const Vec3 &angles)
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

Quat &Quat::fromAxes(const Vec3 &xAxis, const Vec3 &yAxis, const Vec3 &zAxis)
{
    this->fromRotationMatrix(xAxis.x, yAxis.x, zAxis.x,
                             xAxis.y, yAxis.y, zAxis.y,
                             xAxis.z, yAxis.z, zAxis.z);

    return *this;
}

Quat &Quat::fromMatrix(const Mat4 &matrix)
{
    fromRotationMatrix(
            matrix(0, 0), matrix(1, 0), matrix(2, 0),
            matrix(0, 1), matrix(1, 1), matrix(2, 1),
            matrix(0, 2), matrix(1, 2), matrix(2, 2));

    return *this;
}

Quat &Quat::fromRotationMatrix(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
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

    this->normalize();

    return *this;
}

Quat &Quat::slerp(Quat &q2, float changeAmnt)
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

    this->normalize();

    return *this;
}

}
