/*
 * Vector.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <math.h>

namespace re {

class Vec3 {
public:
	float x;
	float y;
	float z;

    Vec3(void);
    explicit Vec3(const float x, const float y, const float z);

    Vec3 &set(float x, float y, float z);
    Vec3 &set(const Vec3 &v);

    float operator [](int index) const;
    float &operator [](int index);

    Vec3 &operator +=(const Vec3 &v);
    Vec3 &operator -=(const Vec3 &v);
    Vec3 &operator *=(const float a);
    Vec3 &operator /=(const float a);

    Vec3 operator +(const Vec3 &v) const;
    Vec3 operator -(const Vec3 &v) const;
    Vec3 operator *(const float a) const;
    Vec3 operator /(const float a) const;

    float operator *(const Vec3 &v) const;

    Vec3 cross(const Vec3 &v) const;
    Vec3 &cross(const Vec3 &a, const Vec3 &b);

    Vec3 &normalize();

    operator float *() const;
    float *toFloatPtr();
    const float *toFloatPtr () const;
};

inline float Vec3::operator [](int index) const
{
    return (&x)[index];
}

inline float &Vec3::operator [](int index)
{
    return (&x)[index];
}

inline Vec3 &Vec3::operator +=(const Vec3 &v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;

    return *this;
}

inline Vec3 &Vec3::operator -=(const Vec3 &v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;

    return *this;
}

inline Vec3 &Vec3::operator *=(const float a)
{
    this->x *= a;
    this->y *= a;
    this->z *= a;

    return *this;
}

inline Vec3 &Vec3::operator /=(const float a)
{
    this->x /= a;
    this->y /= a;
    this->z /= a;

    return *this;
}

inline Vec3 Vec3::operator +(const Vec3 &v) const
{
    return Vec3(x + v.x, y + v.y, z + v.z);
}

inline Vec3 Vec3::operator -(const Vec3 &v) const
{
    return Vec3(x - v.x, y - v.y, z - v.z);
}

inline Vec3 Vec3::operator *(const float a) const
{
    return Vec3(x * a, y * a, z * a);
}

inline Vec3 Vec3::operator /(const float a) const
{
    float inva = 1.0f / a;

    return Vec3(x * inva, y * inva, z * inva);
}

inline float Vec3::operator *(const Vec3 &v) const
{
    return x * v.x + y * v.y + z * v.z;
}

inline Vec3 Vec3::cross(const Vec3 &v) const
{
    return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

inline Vec3 &Vec3::cross(const Vec3 &a, const Vec3 &b)
{
    x = a.y * b.z - a.z * b.y;
    y = a.z * b.x - a.x * b.z;
    z = a.x * b.y - a.y * b.x;

    return *this;
}

inline Vec3 &Vec3::normalize()
{
    float sqrLength, invLength;

    sqrLength = x * x + y * y + z * z;
    invLength = 1.0f / sqrt(sqrLength);
    x *= invLength;
    y *= invLength;
    z *= invLength;

    return *this;
}

inline Vec3::operator float *() const
{
    return (float*)&x;
}

class Vec4 {
public:
    float x;
    float y;
    float z;
    float w;

    Vec4();
    Vec4(const float x, const float y, const float z, const float w);

    Vec4 &set(const float x, const float y, const float z, const float w);
    Vec4 &zero();

    float operator [](int index) const;
    float &operator [](int index);

    /**
     * @brief operator float *
     * 强转时调用
     */
    operator float *() const;

    float *toFloatPtr();
    const float *toFloatPtr () const;
};

inline Vec4 &Vec4::set(const float x, const float y, const float z, const float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;

    return *this;
}

inline Vec4 &Vec4::zero() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;

    return *this;
}

inline float Vec4::operator [](int index) const
{
    return (&x)[index];
}

inline float &Vec4::operator [](int index)
{
    return (&x)[index];
}

inline Vec4::operator float *() const
{
    return (float *)&x;
}

inline float *Vec4::toFloatPtr()
{
    return &x;
}

inline const float *Vec4::toFloatPtr() const
{
    return &x;
}

} // namespace re

#endif /* VECTOR_H_ */
