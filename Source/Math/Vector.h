/*
 * Vector.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <math.h>
#include <cmath>

#include "MathTool.h"

namespace re {

class Quat;

class Vec2 {
public:
    static const Vec2 Zero;

public:
    float x;
    float y;

    Vec2(void);
    Vec2(const float x, const float y);

    Vec2& set(const float x, const float y);
    Vec2& set(const Vec2 &v);
    Vec2& zero(void);

    float operator [](int index) const;
    float& operator [](int index);

    Vec2 operator -() const;
    Vec2 operator +(const Vec2& v) const;
    Vec2 operator -(const Vec2& v) const;
    float operator *(const Vec2& v) const;
    Vec2 operator *(const float a) const;
    Vec2 operator /(const float a) const;

    Vec2& operator +=(const Vec2& v);
    Vec2& operator -=(const Vec2& v);
    Vec2& operator *=(const float a);
    Vec2& operator /=(const Vec2& v);
    Vec2& operator /=(const float a);

    friend Vec2	operator*( const float a, const Vec2 b );

    bool			compare( const Vec2 &a ) const;							// exact compare, no epsilon
    bool			compare( const Vec2 &a, const float epsilon ) const;		// compare with epsilon
    bool			operator==(	const Vec2 &a ) const;						// exact compare, no epsilon
    bool			operator!=(	const Vec2 &a ) const;						// exact compare, no epsilon

    float			length( void ) const;
    float			lengthSqr( void ) const;
    Vec2			normalize( void ) const;
    Vec2&           normalizeSelf( void );

    operator float *() const;
    const float *	toFloatPtr( void ) const;
    float *			toFloatPtr( void );

    void			lerp( const Vec2 &v1, const Vec2 &v2, const float l );
};

inline Vec2::Vec2()
    :x(0), y(0)
{

}

inline Vec2::Vec2(const float x, const float y)
{
    this->x = x;
    this->y = y;
}

inline Vec2 &Vec2::set(const float x, const float y)
{
    this->x = x;
    this->y = y;

    return *this;
}

inline Vec2 &Vec2::set(const Vec2 &v)
{
    *this = v;

    return *this;
}

inline Vec2 &Vec2::zero()
{
    x = y = 0.0f;

    return *this;
}

inline float Vec2::operator [](int index) const
{
    return (&x)[index];
}

inline float &Vec2::operator [](int index)
{
    return (&x)[index];
}

inline Vec2 Vec2::operator -() const
{
    return Vec2(-x, -y);
}

inline Vec2 Vec2::operator +(const Vec2 &v) const
{
    return Vec2(x + v.x, y + v.y);
}

inline Vec2 Vec2::operator -(const Vec2 &v) const
{
    return Vec2(x - v.x, y - v.y);
}

inline float Vec2::operator *(const Vec2 &v) const
{
    return x * v.x + y * v.y;
}

inline Vec2 Vec2::operator *(const float a) const
{
    return Vec2(x * a, y * a);
}

inline Vec2 Vec2::operator /(const float a) const
{
    return Vec2(x / a, y / a);
}

inline Vec2 &Vec2::operator +=(const Vec2 &v)
{
    x += v.x;
    y += v.y;

    return *this;
}

inline Vec2 &Vec2::operator -=(const Vec2 &v)
{
    x -= v.x;
    y -= v.y;

    return *this;
}

inline Vec2 &Vec2::operator *=(const float a)
{
    x *= a;
    y *= a;

    return *this;
}

inline Vec2 &Vec2::operator /=(const Vec2 &v)
{
    x /= v.x;
    y /= v.y;

    return *this;
}

inline Vec2 &Vec2::operator /=(const float a)
{
    x /= a;
    y /= a;

    return *this;
}

inline Vec2 operator*( const float a, const Vec2 b ) {
    return Vec2( b.x * a, b.y * a );
}

inline bool Vec2::compare(const Vec2 &a) const
{
    return ((x == a.x) && (y == a.y));
}

inline bool Vec2::compare(const Vec2 &a, const float epsilon) const
{
    if (fabs(x - a.x) < epsilon) {
        return true;
    }

    if (fabs(y - a.y) < epsilon) {
        return true;
    }

    return false;
}

inline bool Vec2::operator==(const Vec2 &a) const
{
    return this->compare(a);
}

inline bool Vec2::operator!=(const Vec2 &a) const
{
    return !this->compare(a);
}

inline float Vec2::length() const
{
    return sqrt(x *x + y * y);
}

inline float Vec2::lengthSqr() const
{
    return (x * x + y * y);
}

inline Vec2 &Vec2::normalizeSelf()
{
    *this = *this / length();

    return *this;
}

inline Vec2 Vec2::normalize() const
{
    return *this / length();
}

inline const float *Vec2::toFloatPtr() const
{
    return &x;
}

inline float *Vec2::toFloatPtr()
{
    return &x;
}

inline re::Vec2::operator float *() const
{
    return (float*)&x;
}

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

    Vec3            cross(const Vec3 &v) const;
    Vec3&           cross(const Vec3 &a, const Vec3 &b);

    float			length( void ) const;
    float			lengthSqr( void ) const;
    Vec3            normalize() const;
    Vec3&           normalizeSelf();

    operator float *() const;
    float *toFloatPtr();
    const float *toFloatPtr () const;

    Quat toQuat() const;
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
    float inva = 1.0f / a;

    this->x *= inva;
    this->y *= inva;
    this->z *= inva;

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

inline float Vec3::length() const
{
    return sqrt(x * x + y * y + z * z);
}

inline float Vec3::lengthSqr() const
{
    return (x * x + y * y + z * z);
}

inline Vec3 Vec3::normalize() const
{
    return *this / length();
}

inline Vec3 &Vec3::normalizeSelf()
{
    *this = *this / length();

    return *this;
}

inline Vec3::operator float *() const
{
    return (float*)&x;
}

inline float *Vec3::toFloatPtr()
{
    return &x;
}

inline const float *Vec3::toFloatPtr() const
{
    return &x;
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
    Vec4 &set(const Vec4 &source);
    Vec4 &zero();

    Vec4& operator =(const Vec3& source);

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

inline Vec4 &Vec4::set(const Vec4 &source)
{
    this->x = source.x;
    this->y = source.y;
    this->z = source.z;
    this->w = source.w;

    return *this;
}

inline Vec4 &Vec4::zero() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;

    return *this;
}

inline Vec4 &Vec4::operator =(const Vec3 &source)
{
    this->x = source.x;
    this->y = source.y;
    this->z = source.z;
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
