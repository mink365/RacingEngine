/*
 * Matrix.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "Math/Vector.h"
#include <math.h>
#include <string.h>

#include "Math/Math.h"
#include "Quaternion.h"

class Mat4;

// TODO: self (inverseSelf) func unify

namespace re {

class Mat3 {
public:
    Mat3();
    Mat3(float m00, float m01, float m02,
           float m10, float m11, float m12,
           float m20, float m21, float m22);

    Mat3 &zero();
    Mat3 &identity();
    Mat3 &transpose();
    Mat3 &invert();

    const Vec3 	&operator[](int block_index) const;
    Vec3          &operator[](int block_index);

    float getDeterminant();

    Mat4 toMat4();

    operator float *() const;
    float *toFloatPtr();
    const float *toFloatPtr () const;

    std::string toString() const;
private:
    Vec3 mat[3];
};

inline Mat3 &Mat3::zero() {
    memset(mat, 0, sizeof(Mat3));

    return *this;
}

inline Mat3 &Mat3::identity()
{
    mat[0][0] = 1;
    mat[1][1] = 1;
    mat[2][2] = 1;

    return *this;
}


inline const Vec3 &Mat3::operator[](int index) const
{
    //assert( ( index >= 0 ) && ( index < 3 ) );
    return mat[ index ];
}

inline Vec3 &Mat3::operator[](int index)
{
    //assert( ( index >= 0 ) && ( index < 3 ) );
    return mat[ index ];
}

inline Mat3::operator float *() const
{
    return (float *)mat[0];
}

inline std::string Mat3::toString() const
{
    std::string out("Mat3( \n");

    out += StringUtil::Printf("[%f, %f, %f] \n", mat[0][0], mat[0][1], mat[0][2]);
    out += StringUtil::Printf("[%f, %f, %f] \n", mat[1][0], mat[1][1], mat[1][2]);
    out += StringUtil::Printf("[%f, %f, %f] \n", mat[2][0], mat[2][1], mat[2][2]);

    out += ")";

    return out;
}

/**
 * @brief The Mat4 class
 * row-major
 */
class Mat4 {
public:
    static const Mat4 Identity;

public:
    Mat4();

    Mat4(const float m00, const float m01, const float m02, const float m03,
           const float m10, const float m11, const float m12, const float m13,
           const float m20, const float m21, const float m22, const float m23,
           const float m30, const float m31, const float m32, const float m33);

    Mat4 &set(const float m00, const float m01, const float m02, const float m03,
                const float m10, const float m11, const float m12, const float m13,
                const float m20, const float m21, const float m22, const float m23,
                const float m30, const float m31, const float m32, const float m33);

    Mat4 &zero();
    Mat4 &identity();

    Mat4 &setPerspective(const float fov, const float zNear, const float zFar);
    Mat4 &setPerspectiveY(const float fovY, const float aspect, const float zNear, const float zFar);
    Mat4 &setFrustum(const float left, const float right, const float bottom, const float top, const float near, const float far);
    Mat4 &setOrthoFrustum(const float left, const float right, const float bottom, const float top, const float near, const float far);
    Mat4 &lookAt(const Vec3& eye, const Vec3& center, const Vec3& preUp);

    Mat4 &setRotationX(const float angle);
    Mat4 &setRotationY(const float angle);
    Mat4 &setRotationZ(const float angle);

    Mat4 &setTranslation(const float x, const float y, const float z);

    Mat4 &setScaling(float x, float y, float z);

    Vec3 getTranslation() const;
    Vec3 getScaling() const;

    /**
     * @brief fromRTS
     * @param r
     * @param t
     * @param s
     * @return
     * 使用RTS数据给出矩阵
     */
    Mat4 &fromRTS(const Quat &r, const Vec3 &t, const Vec3 &s);

    float *toFloatPtr();
    const float *toFloatPtr() const;

    Mat4 operator*(const float a) const;
    Vec4 operator*(const Vec4 &vec) const;
    Vec3 operator*(const Vec3 &vec) const;
    Mat4 operator*(const Mat4 &r) const;
    Mat4 operator+(const Mat4 &r) const;
    Mat4 operator-(const Mat4 &r) const;
    Mat4 &operator*=(const float r);
    Mat4 &operator*=(const Mat4 &r);
    Mat4 &operator+=(const Mat4 &r);
    Mat4 &operator-=(const Mat4 &r);

    operator float *() const;
    const Vec4 	&operator[](int block_index) const;
    Vec4        &operator[](int block_index);

    float& operator()(size_t row, size_t col);
    const float& operator()(size_t row, size_t col) const;

    float getDeterminant();

    Mat4 inverse() const;
    Mat4 &inverseSelf();

    Mat4 &transpose();

    std::string toString() const;

private:
    Vec4 mat[4];
};

inline Mat4::Mat4(const float m00, const float m01, const float m02, const float m03,
                      const float m10, const float m11, const float m12, const float m13,
                      const float m20, const float m21, const float m22, const float m23,
                      const float m30, const float m31, const float m32, const float m33) {
    mat[0].set(m00, m01, m02, m03);
    mat[1].set(m10, m11, m12, m13);
    mat[2].set(m20, m21, m22, m23);
    mat[3].set(m30, m31, m32, m33);
}

inline Mat4 &Mat4::set(const float m00, const float m01, const float m02, const float m03,
                           const float m10, const float m11, const float m12, const float m13,
                           const float m20, const float m21, const float m22, const float m23,
                           const float m30, const float m31, const float m32, const float m33) {
    mat[0].set(m00, m01, m02, m03);
    mat[1].set(m10, m11, m12, m13);
    mat[2].set(m20, m21, m22, m23);
    mat[3].set(m30, m31, m32, m33);

    return *this;
}

inline Mat4 &Mat4::zero() {
    memset(mat, 0, sizeof(Mat4));

    return *this;
}

inline Mat4 &Mat4::identity() {
    this->zero ();
    mat[0][0] = 1.0f;
    mat[1][1] = 1.0f;
    mat[2][2] = 1.0f;
    mat[3][3] = 1.0f;

    return *this;
}

inline Mat4 &Mat4::setPerspective(const float fov, const float zNear, const float zFar)
{
    this->setPerspectiveY (fov, 1.0f, zNear, zFar);

    return *this;
}

inline Mat4 &Mat4::setPerspectiveY(const float fovY, const float aspect, const float zNear, const float zFar)
{
    // 获取near面上的高的半长
    float top = tan((fovY / 2.0f) * DEG_TO_RAD) * zNear;
    float bottom = -top;
    float right = top * aspect; // 获取near面上的宽的半长
    float left = -right;

    this->setFrustum (left, right, bottom, top, zNear, zFar);

    return *this;
}

inline Mat4 &Mat4::setFrustum(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
    this->zero ();

    mat[0][0] = 2.0f * near / (right - left);
    mat[0][2] = (right + left) / (right - left);

    mat[1][1] = 2.0f * near / (top - bottom);
    mat[1][2] = (top + bottom) / (top - bottom);

    mat[2][2] = -(far + near) / (far - near);
    mat[2][3] = -(2.0f * far * near) / (far - near);

    mat[3][2] = -1.0f;
    mat[3][3] = 0.0f;

    return *this;
}

inline Mat4 &Mat4::setOrthoFrustum(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
    float x_orth = 2 / (right - left);
    float y_orth = 2 / (top - bottom);
    float z_orth = -2 / (far - near);

    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    float tz = -(far + near) / (far - near);

    this->identity ();

    mat[0][0] = x_orth;
    mat[1][1] = y_orth;
    mat[2][2] = z_orth;

    mat[0][3] = tx;
    mat[1][3] = ty;
    mat[2][3] = tz;

    return *this;
}

inline float *Mat4::toFloatPtr()
{
    return mat[0].toFloatPtr();
}

inline const float *Mat4::toFloatPtr() const
{
    return mat[0].toFloatPtr ();
}

inline Mat4 Mat4::operator*(const float a) const
{
    return Mat4(
        mat[0].x * a, mat[0].y * a, mat[0].z * a, mat[0].w * a,
        mat[1].x * a, mat[1].y * a, mat[1].z * a, mat[1].w * a,
        mat[2].x * a, mat[2].y * a, mat[2].z * a, mat[2].w * a,
        mat[3].x * a, mat[3].y * a, mat[3].z * a, mat[3].w * a );
}

inline Vec4 Mat4::operator*(const Vec4 &vec) const
{
    return Vec4(
        mat[ 0 ].x * vec.x + mat[ 0 ].y * vec.y + mat[ 0 ].z * vec.z + mat[ 0 ].w * vec.w,
        mat[ 1 ].x * vec.x + mat[ 1 ].y * vec.y + mat[ 1 ].z * vec.z + mat[ 1 ].w * vec.w,
        mat[ 2 ].x * vec.x + mat[ 2 ].y * vec.y + mat[ 2 ].z * vec.z + mat[ 2 ].w * vec.w,
        mat[ 3 ].x * vec.x + mat[ 3 ].y * vec.y + mat[ 3 ].z * vec.z + mat[ 3 ].w * vec.w );
}

inline Vec3 Mat4::operator*(const Vec3 &vec) const
{
    float s = mat[ 3 ].x * vec.x + mat[ 3 ].y * vec.y + mat[ 3 ].z * vec.z + mat[ 3 ].w;
    if ( s == 0.0f ) {
        return Vec3( 0.0f, 0.0f, 0.0f );
    }
    if ( s == 1.0f ) {
        return Vec3(
            mat[ 0 ].x * vec.x + mat[ 0 ].y * vec.y + mat[ 0 ].z * vec.z + mat[ 0 ].w,
            mat[ 1 ].x * vec.x + mat[ 1 ].y * vec.y + mat[ 1 ].z * vec.z + mat[ 1 ].w,
            mat[ 2 ].x * vec.x + mat[ 2 ].y * vec.y + mat[ 2 ].z * vec.z + mat[ 2 ].w );
    }
    else {
        float invS = 1.0f / s;
        return Vec3(
            (mat[ 0 ].x * vec.x + mat[ 0 ].y * vec.y + mat[ 0 ].z * vec.z + mat[ 0 ].w) * invS,
            (mat[ 1 ].x * vec.x + mat[ 1 ].y * vec.y + mat[ 1 ].z * vec.z + mat[ 1 ].w) * invS,
            (mat[ 2 ].x * vec.x + mat[ 2 ].y * vec.y + mat[ 2 ].z * vec.z + mat[ 2 ].w) * invS );
    }
}

inline Mat4 Mat4::operator *(const Mat4 &r) const
{
    int i, j;
    const float *m1Ptr, *m2Ptr;
    float *dstPtr;
    Mat4 dst;

    m1Ptr = reinterpret_cast<const float *>(this);
    m2Ptr = reinterpret_cast<const float *>(&r);
    dstPtr = reinterpret_cast<float *>(&dst);

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            *dstPtr = m1Ptr[0] * m2Ptr[ 0 * 4 + j ]
                      + m1Ptr[1] * m2Ptr[ 1 * 4 + j ]
                      + m1Ptr[2] * m2Ptr[ 2 * 4 + j ]
                      + m1Ptr[3] * m2Ptr[ 3 * 4 + j ];
            dstPtr++;
        }

        m1Ptr += 4;
    }

    return dst;
}

inline Mat4 Mat4::operator+(const Mat4 &r) const
{
    return Mat4(
        mat[0].x + r[0].x, mat[0].y + r[0].y, mat[0].z + r[0].z, mat[0].w + r[0].w,
        mat[1].x + r[1].x, mat[1].y + r[1].y, mat[1].z + r[1].z, mat[1].w + r[1].w,
        mat[2].x + r[2].x, mat[2].y + r[2].y, mat[2].z + r[2].z, mat[2].w + r[2].w,
        mat[3].x + r[3].x, mat[3].y + r[3].y, mat[3].z + r[3].z, mat[3].w + r[3].w );
}

inline Mat4 Mat4::operator-(const Mat4 &r) const
{
    return Mat4(
        mat[0].x - r[0].x, mat[0].y - r[0].y, mat[0].z - r[0].z, mat[0].w - r[0].w,
        mat[1].x - r[1].x, mat[1].y - r[1].y, mat[1].z - r[1].z, mat[1].w - r[1].w,
        mat[2].x - r[2].x, mat[2].y - r[2].y, mat[2].z - r[2].z, mat[2].w - r[2].w,
        mat[3].x - r[3].x, mat[3].y - r[3].y, mat[3].z - r[3].z, mat[3].w - r[3].w );
}

inline Mat4 &Mat4::operator*=(const float a)
{
    mat[0].x *= a; mat[0].y *= a; mat[0].z *= a; mat[0].w *= a;
    mat[1].x *= a; mat[1].y *= a; mat[1].z *= a; mat[1].w *= a;
    mat[2].x *= a; mat[2].y *= a; mat[2].z *= a; mat[2].w *= a;
    mat[3].x *= a; mat[3].y *= a; mat[3].z *= a; mat[3].w *= a;
    return *this;
}

inline Mat4 &Mat4::operator *=(const Mat4 &r)
{
    *this = (*this) * r;

    return *this;
}

inline Mat4 &Mat4::operator+=(const Mat4 &r)
{
    mat[0].x += r[0].x; mat[0].y += r[0].y; mat[0].z += r[0].z; mat[0].w += r[0].w;
    mat[1].x += r[1].x; mat[1].y += r[1].y; mat[1].z += r[1].z; mat[1].w += r[1].w;
    mat[2].x += r[2].x; mat[2].y += r[2].y; mat[2].z += r[2].z; mat[2].w += r[2].w;
    mat[3].x += r[3].x; mat[3].y += r[3].y; mat[3].z += r[3].z; mat[3].w += r[3].w;
    return *this;
}

inline Mat4 &Mat4::operator-=(const Mat4 &r)
{
    mat[0].x -= r[0].x; mat[0].y -= r[0].y; mat[0].z -= r[0].z; mat[0].w -= r[0].w;
    mat[1].x -= r[1].x; mat[1].y -= r[1].y; mat[1].z -= r[1].z; mat[1].w -= r[1].w;
    mat[2].x -= r[2].x; mat[2].y -= r[2].y; mat[2].z -= r[2].z; mat[2].w -= r[2].w;
    mat[3].x -= r[3].x; mat[3].y -= r[3].y; mat[3].z -= r[3].z; mat[3].w -= r[3].w;
    return *this;
}

inline Mat4::operator float *() const
{
    return (float *)mat[0];
}


inline const Vec4 &Mat4::operator[](int index) const
{
    return mat[ index ];
}

inline Vec4 &Mat4::operator[](int index)
{
    return mat[ index ];
}

inline float& Mat4::operator()(size_t row, size_t col)
{
    return mat[row][col];
}
inline const float& Mat4::operator()(size_t row, size_t col) const
{
    //TODO:
    return mat[row][col];
}

inline std::string Mat4::toString() const
{
    std::string out("Mat4( \n");

    out += StringUtil::Printf("[%f, %f, %f, %f] \n", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
    out += StringUtil::Printf("[%f, %f, %f, %f] \n", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
    out += StringUtil::Printf("[%f, %f, %f, %f] \n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
    out += StringUtil::Printf("[%f, %f, %f, %f] \n", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);

    out += ")";

    return out;
}

} // namespace re

#endif /* MATRIX_H_ */
