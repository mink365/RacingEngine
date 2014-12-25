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

#include "platform.h"

namespace re {

class Quat;

class Vec2 {
public:
    static const Vec2 Zero;

public:
    enum { elem_num = 2 };

public:
    Vec2(void);
    Vec2(const float& x, const float& y);

    Vec2& set(const float& x, const float& y);
    Vec2& set(const Vec2 &v);
    Vec2& zero(void);

    const float& operator [](int index) const;
    float& operator [](int index);

    Vec2 operator -() const;
    Vec2 operator +(const Vec2& v) const;
    Vec2 operator -(const Vec2& v) const;
    Vec2 operator *(const float a) const;
    Vec2 operator /(const float a) const;

    Vec2 operator *(const Vec2& v) const;
    float operator |(const Vec2& v) const;

    Vec2& operator +=(const Vec2& v);
    Vec2& operator -=(const Vec2& v);
    Vec2& operator *=(const float a);
    Vec2& operator *=(const Vec2& v);
    Vec2& operator /=(const float a);
    Vec2& operator /=(const Vec2& v);

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

    std::string toString() const;

public:
    float x;
    float y;
};

inline Vec2::Vec2()
    :x(0), y(0)
{

}

inline Vec2::Vec2(const float& x, const float& y)
{
    this->x = x;
    this->y = y;
}

inline Vec2 &Vec2::set(const float &x, const float &y)
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

inline const float &Vec2::operator [](int index) const
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

inline Vec2 Vec2::operator *(const Vec2 &v) const
{
    return Vec2(x * v.x, y * v.y);
}

inline float Vec2::operator |(const Vec2 &v) const
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

inline Vec2 &Vec2::operator *=(const Vec2 &v)
{
    x *= v.x;
    y *= v.y;

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
    enum { elem_num = 3 };

public:
    Vec3(void);
    Vec3(const float& x, const float& y, const float& z);

    Vec3 &set(const float &x, const float &y, const float &z);
    Vec3 &set(const Vec3 &v);

    const float& operator [](int index) const;
    float &operator [](int index);

    Vec3 &operator +=(const Vec3 &v);
    Vec3 &operator -=(const Vec3 &v);
    Vec3 &operator *=(const float a);
    Vec3 &operator *=(const Vec3 &v);
    Vec3 &operator /=(const float a);
    Vec3 &operator /=(const Vec3 &v);

    Vec3 operator +(const Vec3 &v) const;
    Vec3 operator -(const Vec3 &v) const;
    Vec3 operator *(const float a) const;
    Vec3 operator /(const float a) const;

    Vec3 operator *(const Vec3 &v) const;
    float operator |(const Vec3 &v) const;

    Vec3 const operator+() const
        { return *this; }
    Vec3 const operator-() const
    {
        Vec3 temp(*this);
        temp.x = -x;
        temp.y = -y;
        temp.z = -z;
        return temp;
    }

    friend Vec3	operator*( const float a, const Vec3 b );

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

    std::string toString() const;

public:
    float x;
    float y;
    float z;
};

inline const float &Vec3::operator [](int index) const
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

inline Vec3 &Vec3::operator *=(const Vec3 &v)
{
    this->x *= v.x;
    this->y *= v.y;
    this->z *= v.z;

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

inline Vec3 &Vec3::operator /=(const Vec3 &v)
{
    this->x /= v.x;
    this->y /= v.y;
    this->z /= v.z;

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

inline Vec3 Vec3::operator *(const Vec3 &v) const
{
    return Vec3(x * v.x, y * v.y, z * v.z);
}

inline float Vec3::operator |(const Vec3 &v) const
{
    return x * v.x + y * v.y + z * v.z;
}

inline Vec3	operator*( const float a, const Vec3 b ) {
    return Vec3(b.x * a, b.y * a, b.z * a);
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
    enum { elem_num = 4 };

public:
    Vec4(void);
    Vec4(const float& x, const float& y, const float& z, const float& w);

    Vec4 &set(const float& x, const float& y, const float& z, const float& w);
    Vec4 &set(const Vec4 &source);
    Vec4 &zero(void);

    Vec4 &operator +=(const Vec4 &v);
    Vec4 &operator -=(const Vec4 &v);
    Vec4 &operator *=(const float a);
    Vec4 &operator *=(const Vec4 &v);
    Vec4 &operator /=(const float a);
    Vec4 &operator /=(const Vec4 &v);

    Vec4 operator +(const Vec4 &v) const;
    Vec4 operator -(const Vec4 &v) const;
    Vec4 operator *(const float a) const;
    Vec4 operator /(const float a) const;

    float operator |(const Vec4 &v) const;
    Vec4 operator *(const Vec4 &v) const;

    Vec4& operator =(const Vec3& source);

    const float &operator [](int index) const;
    float &operator [](int index);

    /**
     * @brief operator float *
     */
    operator float *() const;

    float *toFloatPtr();
    const float *toFloatPtr () const;

    std::string toString() const;

public:
    float x;
    float y;
    float z;
    float w;
};

inline Vec4 &Vec4::set(const float &x, const float &y, const float &z, const float &w) {
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

inline Vec4 &Vec4::zero(void) {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;

    return *this;
}

inline Vec4 &Vec4::operator +=(const Vec4 &v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    this->w += v.w;

    return *this;
}

inline Vec4 &Vec4::operator -=(const Vec4 &v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    this->w -= v.w;

    return *this;
}

inline Vec4 &Vec4::operator *=(const float a)
{
    this->x *= a;
    this->y *= a;
    this->z *= a;
    this->w *= a;

    return *this;
}

inline Vec4 &Vec4::operator *=(const Vec4 &v)
{
    this->x *= v.x;
    this->y *= v.y;
    this->z *= v.z;
    this->w *= v.w;

    return *this;
}

inline Vec4 &Vec4::operator /=(const float a)
{
    this->x /= a;
    this->y /= a;
    this->z /= a;
    this->w /= a;

    return *this;
}

inline Vec4 &Vec4::operator /=(const Vec4 &v)
{
    this->x /= v.x;
    this->y /= v.z;
    this->z /= v.y;
    this->w /= v.w;

    return *this;
}

inline Vec4 Vec4::operator +(const Vec4 &v) const
{
    Vec4 result(this->x + v.x,
                this->y + v.y,
                this->z + v.z,
                this->w + v.w);

    return result;
}

inline Vec4 Vec4::operator -(const Vec4 &v) const
{
    Vec4 result(this->x - v.x,
                this->y - v.y,
                this->z - v.z,
                this->w - v.w);

    return result;
}

inline Vec4 Vec4::operator *(const float a) const
{
    Vec4 result(this->x * a,
                this->y * a,
                this->z * a,
                this->w * a);

    return result;
}

inline Vec4 Vec4::operator /(const float a) const
{
    Vec4 result(this->x / a,
                this->y / a,
                this->z / a,
                this->w / a);

    return result;
}

inline Vec4 Vec4::operator *(const Vec4 &v) const
{
    return Vec4(x * v.x, y * v.y, z * v.z, w * v.w);
}

inline float Vec4::operator |(const Vec4 &v) const
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

inline Vec4 &Vec4::operator =(const Vec3 &source)
{
    this->x = source.x;
    this->y = source.y;
    this->z = source.z;
    this->w = 0;

    return *this;
}

inline const float& Vec4::operator [](int index) const
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
