/*
 * Vector.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <math.h>

class reVec3 {
public:
	float x;
	float y;
	float z;

	reVec3(void);
    explicit reVec3(const float x, const float y, const float z);

    reVec3 &set(float x, float y, float z);
    reVec3 &set(const reVec3 &v);

    float operator [](int index) const;
    float &operator [](int index);

    reVec3 &operator +=(const reVec3 &v);
    reVec3 &operator -=(const reVec3 &v);
    reVec3 &operator *=(const float a);
    reVec3 &operator /=(const float a);

    reVec3 operator +(const reVec3 &v) const;
    reVec3 operator -(const reVec3 &v) const;
    reVec3 operator *(const float a) const;
    reVec3 operator /(const float a) const;

    float operator *(const reVec3 &v) const;

    reVec3 cross(const reVec3 &v) const;
    reVec3 &cross(const reVec3 &a, const reVec3 &b);

    reVec3 &normalize();

    operator float *() const;
    float *toFloatPtr();
    const float *toFloatPtr () const;
};

inline float reVec3::operator [](int index) const
{
    return (&x)[index];
}

inline float &reVec3::operator [](int index)
{
    return (&x)[index];
}

inline reVec3 &reVec3::operator +=(const reVec3 &v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;

    return *this;
}

inline reVec3 &reVec3::operator -=(const reVec3 &v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;

    return *this;
}

inline reVec3 &reVec3::operator *=(const float a)
{
    this->x *= a;
    this->y *= a;
    this->z *= a;

    return *this;
}

inline reVec3 &reVec3::operator /=(const float a)
{
    this->x /= a;
    this->y /= a;
    this->z /= a;

    return *this;
}

inline reVec3 reVec3::operator +(const reVec3 &v) const
{
    return reVec3(x + v.x, y + v.y, z + v.z);
}

inline reVec3 reVec3::operator -(const reVec3 &v) const
{
    return reVec3(x - v.x, y - v.y, z - v.z);
}

inline reVec3 reVec3::operator *(const float a) const
{
    return reVec3(x * a, y * a, z * a);
}

inline reVec3 reVec3::operator /(const float a) const
{
    float inva = 1.0f / a;

    return reVec3(x * inva, y * inva, z * inva);
}

inline float reVec3::operator *(const reVec3 &v) const
{
    return x * v.x + y * v.y + z * v.z;
}

inline reVec3 reVec3::cross(const reVec3 &v) const
{
    return reVec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

inline reVec3 &reVec3::cross(const reVec3 &a, const reVec3 &b)
{
    x = a.y * b.z - a.z * b.y;
    y = a.z * b.x - a.x * b.z;
    z = a.x * b.y - a.y * b.x;

    return *this;
}

inline reVec3 &reVec3::normalize()
{
    float sqrLength, invLength;

    sqrLength = x * x + y * y + z * z;
    invLength = 1.0f / sqrt(sqrLength);
    x *= invLength;
    y *= invLength;
    z *= invLength;

    return *this;
}

inline reVec3::operator float *() const
{
    return (float*)&x;
}

class reVec4 {
public:
    float x;
    float y;
    float z;
    float w;

    reVec4();
    reVec4(const float x, const float y, const float z, const float w);

    reVec4 &set(const float x, const float y, const float z, const float w);
    reVec4 &zero();

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

inline reVec4 &reVec4::set(const float x, const float y, const float z, const float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;

    return *this;
}

inline reVec4 &reVec4::zero() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;

    return *this;
}

inline float reVec4::operator [](int index) const
{
    return (&x)[index];
}

inline float &reVec4::operator [](int index)
{
    return (&x)[index];
}

inline reVec4::operator float *() const
{
    return (float *)&x;
}

inline float *reVec4::toFloatPtr()
{
    return &x;
}

inline const float *reVec4::toFloatPtr() const
{
    return &x;
}

#endif /* VECTOR_H_ */
