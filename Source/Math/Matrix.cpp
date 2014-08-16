/*
 * Matrix.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */


#include "Math/Matrix.h"

#include <cmath>
#include <algorithm>

namespace re {

Mat3::Mat3()
{
    this->identity();
}

Mat3::Mat3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
    mat[0].set(m00, m01, m02);
    mat[1].set(m10, m11, m12);
    mat[2].set(m20, m21, m22);
}

float Mat3::getDeterminant()
{
    return mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) -
           mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
           mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
}

Mat4 Mat3::toMat4()
{
    return Mat4(mat[0][0], mat[0][1], mat[0][2], 0.0f,
                  mat[1][0], mat[1][1], mat[1][2], 0.0f,
                  mat[2][0], mat[2][1], mat[2][2], 0.0f,
            0.0f,      0.0f,      0.0f,      1.0f);
}

float *Mat3::toFloatPtr()
{
    return this->mat[0].toFloatPtr();
}

const float *Mat3::toFloatPtr() const
{
    return this->mat[0].toFloatPtr();
}

Mat3 &Mat3::transpose()
{
    std::swap(mat[0][1], mat[1][0]);
    std::swap(mat[0][2], mat[2][0]);
    std::swap(mat[1][2], mat[2][1]);

    return *this;
}

Mat3 &Mat3::invert()
{
    float determinant, invDeterminant;
    float tmp[9];

    tmp[0] = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
    tmp[1] = mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2];
    tmp[2] = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
    tmp[3] = mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2];
    tmp[4] = mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0];
    tmp[5] = mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2];
    tmp[6] = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];
    tmp[7] = mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1];
    tmp[8] = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

    // check determinant if it is 0
    determinant = mat[0][0] * tmp[0] + mat[0][1] * tmp[3] + mat[0][2] * tmp[6];
    if(fabs(determinant) <= 0.00001f)
    {
        return identity(); // cannot inverse, make it idenety matrix
    }

    // divide by the determinant
    invDeterminant = 1.0f / determinant;
    mat[0][0] = invDeterminant * tmp[0];
    mat[0][1] = invDeterminant * tmp[1];
    mat[0][2] = invDeterminant * tmp[2];
    mat[1][0] = invDeterminant * tmp[3];
    mat[1][1] = invDeterminant * tmp[4];
    mat[1][2] = invDeterminant * tmp[5];
    mat[2][0] = invDeterminant * tmp[6];
    mat[2][1] = invDeterminant * tmp[7];
    mat[2][2] = invDeterminant * tmp[8];

    return *this;
}


Mat4::Mat4() {
    this->identity();
}

Mat4 &Mat4::setRotationX(const float angle)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    this->set (1, 0,     0,    0,
               0, cosA, -sinA, 0,
               0, sinA,  cosA, 0,
               0, 0,     0,    1);

    return *this;
}

Mat4 &Mat4::setRotationY(const float angle)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    this->set (cosA,  0,  -sinA,  0,
               0,     1,  0,      0,
               sinA,  0,  cosA,   0,
               0,     0,  0,      1);

    return *this;
}

Mat4 &Mat4::setRotationZ(const float angle)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    this->set (cosA, -sinA, 0,  0,
               sinA,  cosA, 0,  0,
               0,     0,    1,  0,
               0,     0,    0,  1);

    return *this;
}

Mat4 &Mat4::setTranslation(const float x, const float y, const float z)
{
    this->identity();
    mat[0][3] = x;
    mat[1][3] = y;
    mat[2][3] = z;

    return *this;
}

Mat4 &Mat4::setScaling(float x, float y, float z)
{
    this->identity();

    mat[0][0] = x;
    mat[1][1] = y;
    mat[2][2] = z;

    return *this;
}

Vec3 Mat4::getTranslation() const
{
    Vec3 v;
    v.set(mat[0][3], mat[1][3], mat[2][3]);

    return v;
}

Vec3 Mat4::getScaling() const
{
    Vec3 v;

    v.set(mat[0][0], mat[1][1], mat[2][2]);

    return v;
}

Mat4 &Mat4::fromRTS(const Quat &r, const Vec3 &t, const Vec3 &s)
{
    Mat4 tempRotateMatrix = r.toMat4();

    Mat4 tempScalingMatrix;
    tempScalingMatrix.setScaling(s.x, s.y, s.z);

    Mat4 tempTranslationMatrix;
    tempTranslationMatrix.setTranslation(t.x, t.y, t.z);

    // TM * RM * SM
    *this = tempTranslationMatrix * tempRotateMatrix * tempScalingMatrix;

    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// compute cofactor of 3x3 minor matrix without sign
// input params are 9 elements of the minor matrix
// NOTE: The caller must know its sign.
///////////////////////////////////////////////////////////////////////////////
inline float getCofactor(float m0, float m1, float m2,
                         float m3, float m4, float m5,
                         float m6, float m7, float m8)
{
    return m0 * (m4 * m8 - m5 * m7) -
           m1 * (m3 * m8 - m5 * m6) +
           m2 * (m3 * m7 - m4 * m6);
}

///////////////////////////////////////////////////////////////////////////////
// transpose 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
Mat4& Mat4::transpose()
{
    std::swap(mat[0][1],  mat[1][0]);
    std::swap(mat[0][2],  mat[2][0]);
    std::swap(mat[0][3],  mat[2][0]);
    std::swap(mat[1][2],  mat[2][1]);
    std::swap(mat[1][3],  mat[2][1]);
    std::swap(mat[2][3],  mat[2][2]);

    return *this;
}

Mat4 Mat4::inverse() const
{
    Mat4 temp = *this;

    temp.inverseSelf();

    return temp;
}

#define MATRIX_INVERSE_EPSILON		1e-14

Mat4& Mat4::inverseSelf()
{
    // 84+4+16 = 104 multiplications
    //			   1 division
    double det, invDet;

    // 2x2 sub-determinants required to calculate 4x4 determinant
    float det2_01_01 = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    float det2_01_02 = mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0];
    float det2_01_03 = mat[0][0] * mat[1][3] - mat[0][3] * mat[1][0];
    float det2_01_12 = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
    float det2_01_13 = mat[0][1] * mat[1][3] - mat[0][3] * mat[1][1];
    float det2_01_23 = mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2];

    // 3x3 sub-determinants required to calculate 4x4 determinant
    float det3_201_012 = mat[2][0] * det2_01_12 - mat[2][1] * det2_01_02 + mat[2][2] * det2_01_01;
    float det3_201_013 = mat[2][0] * det2_01_13 - mat[2][1] * det2_01_03 + mat[2][3] * det2_01_01;
    float det3_201_023 = mat[2][0] * det2_01_23 - mat[2][2] * det2_01_03 + mat[2][3] * det2_01_02;
    float det3_201_123 = mat[2][1] * det2_01_23 - mat[2][2] * det2_01_13 + mat[2][3] * det2_01_12;

    det = ( - det3_201_123 * mat[3][0] + det3_201_023 * mat[3][1] - det3_201_013 * mat[3][2] + det3_201_012 * mat[3][3] );

    if ( fabs( det ) < MATRIX_INVERSE_EPSILON ) {
        return identity(); // cannot inverse, make it idenety matrix
    }

    invDet = 1.0f / det;

    // remaining 2x2 sub-determinants
    float det2_03_01 = mat[0][0] * mat[3][1] - mat[0][1] * mat[3][0];
    float det2_03_02 = mat[0][0] * mat[3][2] - mat[0][2] * mat[3][0];
    float det2_03_03 = mat[0][0] * mat[3][3] - mat[0][3] * mat[3][0];
    float det2_03_12 = mat[0][1] * mat[3][2] - mat[0][2] * mat[3][1];
    float det2_03_13 = mat[0][1] * mat[3][3] - mat[0][3] * mat[3][1];
    float det2_03_23 = mat[0][2] * mat[3][3] - mat[0][3] * mat[3][2];

    float det2_13_01 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
    float det2_13_02 = mat[1][0] * mat[3][2] - mat[1][2] * mat[3][0];
    float det2_13_03 = mat[1][0] * mat[3][3] - mat[1][3] * mat[3][0];
    float det2_13_12 = mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1];
    float det2_13_13 = mat[1][1] * mat[3][3] - mat[1][3] * mat[3][1];
    float det2_13_23 = mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2];

    // remaining 3x3 sub-determinants
    float det3_203_012 = mat[2][0] * det2_03_12 - mat[2][1] * det2_03_02 + mat[2][2] * det2_03_01;
    float det3_203_013 = mat[2][0] * det2_03_13 - mat[2][1] * det2_03_03 + mat[2][3] * det2_03_01;
    float det3_203_023 = mat[2][0] * det2_03_23 - mat[2][2] * det2_03_03 + mat[2][3] * det2_03_02;
    float det3_203_123 = mat[2][1] * det2_03_23 - mat[2][2] * det2_03_13 + mat[2][3] * det2_03_12;

    float det3_213_012 = mat[2][0] * det2_13_12 - mat[2][1] * det2_13_02 + mat[2][2] * det2_13_01;
    float det3_213_013 = mat[2][0] * det2_13_13 - mat[2][1] * det2_13_03 + mat[2][3] * det2_13_01;
    float det3_213_023 = mat[2][0] * det2_13_23 - mat[2][2] * det2_13_03 + mat[2][3] * det2_13_02;
    float det3_213_123 = mat[2][1] * det2_13_23 - mat[2][2] * det2_13_13 + mat[2][3] * det2_13_12;

    float det3_301_012 = mat[3][0] * det2_01_12 - mat[3][1] * det2_01_02 + mat[3][2] * det2_01_01;
    float det3_301_013 = mat[3][0] * det2_01_13 - mat[3][1] * det2_01_03 + mat[3][3] * det2_01_01;
    float det3_301_023 = mat[3][0] * det2_01_23 - mat[3][2] * det2_01_03 + mat[3][3] * det2_01_02;
    float det3_301_123 = mat[3][1] * det2_01_23 - mat[3][2] * det2_01_13 + mat[3][3] * det2_01_12;

    mat[0][0] =	- det3_213_123 * invDet;
    mat[1][0] = + det3_213_023 * invDet;
    mat[2][0] = - det3_213_013 * invDet;
    mat[3][0] = + det3_213_012 * invDet;

    mat[0][1] = + det3_203_123 * invDet;
    mat[1][1] = - det3_203_023 * invDet;
    mat[2][1] = + det3_203_013 * invDet;
    mat[3][1] = - det3_203_012 * invDet;

    mat[0][2] = + det3_301_123 * invDet;
    mat[1][2] = - det3_301_023 * invDet;
    mat[2][2] = + det3_301_013 * invDet;
    mat[3][2] = - det3_301_012 * invDet;

    mat[0][3] = - det3_201_123 * invDet;
    mat[1][3] = + det3_201_023 * invDet;
    mat[2][3] = - det3_201_013 * invDet;
    mat[3][3] = + det3_201_012 * invDet;

    return *this;
}


///////////////////////////////////////////////////////////////////////////////
// return determinant of 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
float Mat4::getDeterminant()
{
    return mat[0][0] * getCofactor(mat[1][1],mat[1][2],mat[1][3], mat[2][1],mat[2][2],mat[2][3], mat[3][1],mat[3][2],mat[3][3]) -
           mat[0][1] * getCofactor(mat[1][0],mat[1][2],mat[1][3], mat[2][0],mat[2][2],mat[2][3], mat[3][0],mat[3][2],mat[3][3]) +
           mat[0][2] * getCofactor(mat[1][0],mat[1][1],mat[1][3], mat[2][0],mat[2][1], mat[2][3], mat[3][0],mat[3][1],mat[3][3]) -
           mat[0][2] * getCofactor(mat[1][0],mat[1][1],mat[1][2], mat[2][0],mat[2][1], mat[2][2], mat[3][0],mat[3][1],mat[3][2]);
}


Mat4 &Mat4::lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &preUp)
{
    // See the OpenGL GLUT documentation for gluLookAt for a description
    // of the algorithm. We implement it in a straightforward way:

    Vec3 forward = (center - eye).normalize();

    // compute s = f x up (x means "cross product")
    Vec3 side = forward.cross(preUp).normalize();

    // compute u = s x f
    Vec3 up = side.cross(forward).normalize();

    mat[0][0] = side.x;
    mat[0][1] = side.y;
    mat[0][2] = side.z;
    mat[0][3] = 0.0f;

    mat[1][0] = up.x;
    mat[1][1] = up.y;
    mat[1][2] = up.z;
    mat[1][3] = 0.0f;

    mat[2][0] = -forward.x;
    mat[2][1] = -forward.y;
    mat[2][2] = -forward.z;
    mat[2][3] = 0.0f;

    mat[3][0] = 0.0f;
    mat[3][1] = 0.0f;
    mat[3][2] = 0.0f;
    mat[3][3] = 1.0f;

    for (int i=0 ; i<3 ; i++) {
        int mi = i;
        mat[mi][3] += mat[mi][0] * -eye.x + mat[mi][1] * -eye.y + mat[mi][2] * -eye.z;
    }

    return *this;
}

}
