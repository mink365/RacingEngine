/*
 * Matrix.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "math/Vector.h"
#include <math.h>
#include <string.h>

#include "math/MathTool.h"
#include "Quaternion.h"

class reMat4;

class reMat3 {
public:
    reMat3();
    reMat3(float m00, float m01, float m02,
           float m10, float m11, float m12,
           float m20, float m21, float m22);

    reMat3 &zero();
    reMat3 &identity();
    reMat3 &transpose();
    reMat3 &invert();

    const reVec3 	&operator[](int block_index) const;
    reVec3          &operator[](int block_index);

    float getDeterminant();

    reMat4 toMat4();

    operator float *() const;
    float *toFloatPtr();
    const float *toFloatPtr () const;
private:
    reVec3 mat[3];
};

inline reMat3 &reMat3::zero() {
    memset(mat, 0, sizeof(reMat3));

    return *this;
}

inline reMat3 &reMat3::identity()
{
    mat[0][0] = 1;
    mat[1][1] = 1;
    mat[2][2] = 1;

    return *this;
}


inline const reVec3 &reMat3::operator[](int index) const
{
    //assert( ( index >= 0 ) && ( index < 3 ) );
    return mat[ index ];
}

inline reVec3 &reMat3::operator[](int index)
{
    //assert( ( index >= 0 ) && ( index < 3 ) );
    return mat[ index ];
}

/**
 * @brief The reMat4 class
 * 列主序(column major)
 */
class reMat4 {
public:
    reMat4();

    reMat4(const float m00, const float m01, const float m02, const float m03,
           const float m10, const float m11, const float m12, const float m13,
           const float m20, const float m21, const float m22, const float m23,
           const float m30, const float m31, const float m32, const float m33);

    reMat4 &set(const float m00, const float m01, const float m02, const float m03,
                const float m10, const float m11, const float m12, const float m13,
                const float m20, const float m21, const float m22, const float m23,
                const float m30, const float m31, const float m32, const float m33);

    reMat4 &zero();
    reMat4 &identity();

    reMat4 &setPerspective(const float fov, const float zNear, const float zFar);
    reMat4 &setPerspectiveY(const float fovY, const float aspect, const float zNear, const float zFar);
    reMat4 &setFrustum(const float left, const float right, const float bottom, const float top, const float near, const float far);
    reMat4 &setOrthoFrustum(const float left, const float right, const float bottom, const float top, const float near, const float far);
    reMat4 &lookAt(reVec3 eye, reVec3 center, reVec3 up);

    reMat4 &setRotationX(const float angle);
    reMat4 &setRotationY(const float angle);
    reMat4 &setRotationZ(const float angle);

    reMat4 &setTranslation(const float x, const float y, const float z);

    reMat4 &setScaling(float x, float y, float z);

    /**
     * @brief fromRTS
     * @param r
     * @param t
     * @param s
     * @return
     * 使用RTS数据给出矩阵
     */
    reMat4 &fromRTS(const reQuat &r, const reVec3 &t, const reVec3 &s);

    float *toFloatPtr();
    const float *toFloatPtr () const;

    reMat4 operator*(const reMat4 &r) const;
    reMat4 &operator *=(const reMat4 &r);

    operator float *() const;
    const reVec4 	&operator[](int block_index) const;
    reVec4          &operator[](int block_index);

    float getDeterminant();

    reMat4 &invertGeneral();
    reMat4 &invertAffine();
    reMat4 &invert();
    reMat4 &transpose();
private:
    reVec4 mat[4];
};

inline reMat4::reMat4(const float m00, const float m01, const float m02, const float m03,
                      const float m10, const float m11, const float m12, const float m13,
                      const float m20, const float m21, const float m22, const float m23,
                      const float m30, const float m31, const float m32, const float m33) {
    mat[0].set(m00, m01, m02, m03);
    mat[1].set(m10, m11, m12, m13);
    mat[2].set(m20, m21, m22, m23);
    mat[3].set(m30, m31, m32, m33);
}

inline reMat4 &reMat4::set(const float m00, const float m01, const float m02, const float m03,
                           const float m10, const float m11, const float m12, const float m13,
                           const float m20, const float m21, const float m22, const float m23,
                           const float m30, const float m31, const float m32, const float m33) {
    mat[0].set(m00, m01, m02, m03);
    mat[1].set(m10, m11, m12, m13);
    mat[2].set(m20, m21, m22, m23);
    mat[3].set(m30, m31, m32, m33);

    return *this;
}

inline reMat4 &reMat4::zero() {
    memset(mat, 0, sizeof(reMat4));

    return *this;
}

inline reMat4 &reMat4::identity() {
    this->zero ();
    mat[0][0] = 1.0f;
    mat[1][1] = 1.0f;
    mat[2][2] = 1.0f;
    mat[3][3] = 1.0f;

    return *this;
}

inline reMat4 &reMat4::setPerspective(const float fov, const float zNear, const float zFar)
{
    this->setPerspectiveY (fov, 1.0f, zNear, zFar);

    return *this;
}

inline reMat4 &reMat4::setPerspectiveY(const float fovY, const float aspect, const float zNear, const float zFar)
{
    // 获取near面上的高的半长
    float top = tan((fovY / 2.0f) * DEG_TO_RAD) * zNear;
    float bottom = -top;
    float right = top * aspect; // 获取near面上的宽的半长
    float left = -right;

    this->setFrustum (left, right, bottom, top, zNear, zFar);

    return *this;
}

inline reMat4 &reMat4::setFrustum(const float left, const float right, const float bottom, const float top, const float near, const float far)
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

inline reMat4 &reMat4::setOrthoFrustum(const float left, const float right, const float bottom, const float top, const float near, const float far)
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

inline float *reMat4::toFloatPtr()
{
    return mat[0].toFloatPtr();
}

inline const float *reMat4::toFloatPtr() const
{
    return mat[0].toFloatPtr ();
}

inline reMat4 reMat4::operator *(const reMat4 &r) const
{
    int i, j;
    const float *m1Ptr, *m2Ptr;
    float *dstPtr;
    reMat4 dst;

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

inline reMat4 &reMat4::operator *=(const reMat4 &r)
{
    *this = (*this) * r;

    return *this;
}

inline reMat4::operator float *() const
{
    return (float *)mat[0];
}


inline const reVec4 &reMat4::operator[](int index) const
{
    return mat[ index ];
}

inline reVec4 &reMat4::operator[](int index)
{
    return mat[ index ];
}

#endif /* MATRIX_H_ */
