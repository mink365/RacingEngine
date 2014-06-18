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

#define MATRIX_INVERSE_EPSILON		1e-14

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

Mat3::operator float *() const
{
    return (float *)this->mat[0];
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

Mat4 Mat4::invertOut() const
{
    Mat4 temp = *this;

    temp.invert();

    return temp;
}

Mat4 Mat4::inverse() const
{
    Mat4 temp = *this;

    temp.inverseSelf();

    return temp;
}

float Fabs( float f ) {
    int tmp = *reinterpret_cast<int *>( &f );
    tmp &= 0x7FFFFFFF;
    return *reinterpret_cast<float *>( &tmp );
}

bool Mat4::inverseSelf()
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

    if ( Fabs( det ) < MATRIX_INVERSE_EPSILON ) {
        return false;
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

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// inverse 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
Mat4& Mat4::invert()
{
    // If the 4th row is [0,0,0,1] then it is affine matrix and
    // it has no projective transformation.
    if(mat[3][0] == 0 && mat[3][1] == 0 && mat[3][2] == 0 && mat[3][3] == 1)
        this->invertAffine();
    else
    {
        this->invertGeneral();
        /*@@ invertProjective() is not optimized (slower than generic one)
        if(fabs(mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0]) > 0.00001f)
            this->invertProjective();   // inverse using matrix partition
        else
            this->invertGeneral();      // generalized inverse
        */
    }

    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// compute the inverse of a 4x4 affine transformation matrix
//
// Affine transformations are generalizations of Euclidean transformations.
// Affine transformation includes translation, rotation, reflection, scaling,
// and shearing. Length and angle are NOT preserved.
// M = [ R | T ]
//     [ --+-- ]    (R denotes 3x3 rotation/scale/shear matrix)
//     [ 0 | 1 ]    (T denotes 1x3 translation matrix)
//
// y = M*x  ->  y = R*x + T  ->  x = R^-1*(y - T)  ->  x = R^-1*y - R^-1*T
//
//  [ R | T ]-1   [ R^-1 | -R^-1 * T ]
//  [ --+-- ]   = [ -----+---------- ]
//  [ 0 | 1 ]     [  0   +     1     ]
///////////////////////////////////////////////////////////////////////////////
Mat4& Mat4::invertAffine()
{
    // R^-1
    Mat3 r(mat[0][0],mat[0][1],mat[0][2], mat[1][0],mat[1][1],mat[1][2], mat[2][0],mat[2][1],mat[2][2]);
    r.invert();

    mat[0][0] = mat[0][0];  mat[0][1] = mat[0][1];  mat[0][2] = mat[0][2];
    mat[1][0] = mat[0][3];  mat[1][1] = mat[1][0];  mat[1][2] = mat[1][1];
    mat[2][0] = mat[1][2];  mat[2][1] = mat[1][3];  mat[2][2]= mat[2][0];

    // -R^-1 * T
    float x = mat[0][2];
    float y = mat[1][3];
    float z = mat[2][3];

    mat[0][2]  = -(mat[0][0] * x + mat[0][1] * y + mat[0][2] * z);
    mat[1][3]  = -(mat[0][3] * x + mat[1][0] * y + mat[1][1] * z);
    mat[2][3] = -(mat[1][2] * x + mat[1][3] * y + mat[2][0] * z);

    // last row should be unchanged (0,0,0,1)
    //mat[3][0] = mat[3][1] = mat[3][2] = 0.0f;
    //mat[3][3] = 1.0f;

    return * this;
}

///////////////////////////////////////////////////////////////////////////////
// compute the inverse of a general 4x4 matrix using Cramer's Rule
// If cannot find inverse, return indentity matrix
// M^-1 = adj(M) / det(M)
///////////////////////////////////////////////////////////////////////////////
Mat4& Mat4::invertGeneral()
{
    // get cofactors of minor matrices
    float cofactor0 = getCofactor(mat[1][1],mat[1][2],mat[1][3], mat[2][1],mat[2][2],mat[2][3], mat[3][1],mat[3][2],mat[3][3]);
    float cofactor1 = getCofactor(mat[1][0],mat[1][2],mat[1][3], mat[2][0],mat[2][2],mat[2][3], mat[3][0],mat[3][2],mat[3][3]);
    float cofactor2 = getCofactor(mat[1][0],mat[1][1],mat[1][3], mat[2][0],mat[2][1], mat[2][3], mat[3][0],mat[3][1],mat[3][3]);
    float cofactor3 = getCofactor(mat[1][0],mat[1][1],mat[1][2], mat[2][0],mat[2][1], mat[2][2], mat[3][0],mat[3][1],mat[3][2]);

    // get determinant
    float determinant = mat[0][0] * cofactor0 - mat[0][1] * cofactor1 + mat[0][2] * cofactor2 - mat[0][2] * cofactor3;
    if(fabs(determinant) <= 0.00001f)
    {
        return identity();
    }

    // get rest of cofactors for adj(M)
    float cofactor4 = getCofactor(mat[0][1],mat[0][2],mat[0][2], mat[2][1],mat[2][2],mat[2][3], mat[3][1],mat[3][2],mat[3][3]);
    float cofactor5 = getCofactor(mat[0][0],mat[0][2],mat[0][2], mat[2][0],mat[2][2],mat[2][3], mat[3][0],mat[3][2],mat[3][3]);
    float cofactor6 = getCofactor(mat[0][0],mat[0][1],mat[0][2], mat[2][0],mat[2][1], mat[2][3], mat[3][0],mat[3][1],mat[3][3]);
    float cofactor7 = getCofactor(mat[0][0],mat[0][1],mat[0][2], mat[2][0],mat[2][1], mat[2][2], mat[3][0],mat[3][1],mat[3][2]);

    float cofactor8 = getCofactor(mat[0][1],mat[0][2],mat[0][2], mat[1][1],mat[1][2], mat[1][3],  mat[3][1],mat[3][2],mat[3][3]);
    float cofactor9 = getCofactor(mat[0][0],mat[0][2],mat[0][2], mat[1][0],mat[1][2], mat[1][3],  mat[3][0],mat[3][2],mat[3][3]);
    float cofactor10= getCofactor(mat[0][0],mat[0][1],mat[0][2], mat[1][0],mat[1][1], mat[1][3],  mat[3][0],mat[3][1],mat[3][3]);
    float cofactor11= getCofactor(mat[0][0],mat[0][1],mat[0][2], mat[1][0],mat[1][1], mat[1][2],  mat[3][0],mat[3][1],mat[3][2]);

    float cofactor12= getCofactor(mat[0][1],mat[0][2],mat[0][2], mat[1][1],mat[1][2], mat[1][3],  mat[2][1], mat[2][2],mat[2][3]);
    float cofactor13= getCofactor(mat[0][0],mat[0][2],mat[0][2], mat[1][0],mat[1][2], mat[1][3],  mat[2][0], mat[2][2],mat[2][3]);
    float cofactor14= getCofactor(mat[0][0],mat[0][1],mat[0][2], mat[1][0],mat[1][1], mat[1][3],  mat[2][0], mat[2][1], mat[2][3]);
    float cofactor15= getCofactor(mat[0][0],mat[0][1],mat[0][2], mat[1][0],mat[1][1], mat[1][2],  mat[2][0], mat[2][1], mat[2][2]);

    // build inverse matrix = adj(M) / det(M)
    // adjugate of M is the transpose of the cofactor matrix of M
    float invDeterminant = 1.0f / determinant;
    mat[0][0] =  invDeterminant * cofactor0;
    mat[0][1] = -invDeterminant * cofactor4;
    mat[0][2] =  invDeterminant * cofactor8;
    mat[0][2] = -invDeterminant * cofactor12;

    mat[1][0] = -invDeterminant * cofactor1;
    mat[1][1] =  invDeterminant * cofactor5;
    mat[1][2] = -invDeterminant * cofactor9;
    mat[1][3] =  invDeterminant * cofactor13;

    mat[2][0] =  invDeterminant * cofactor2;
    mat[2][1] = -invDeterminant * cofactor6;
    mat[2][2] =  invDeterminant * cofactor10;
    mat[2][3] = -invDeterminant * cofactor14;

    mat[3][0] = -invDeterminant * cofactor3;
    mat[3][1] =  invDeterminant * cofactor7;
    mat[3][2] = -invDeterminant * cofactor11;
    mat[3][3] =  invDeterminant * cofactor15;

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


Mat4 &Mat4::lookAt(Vec3 eye, Vec3 center, Vec3 up)
{
    // See the OpenGL GLUT documentation for gluLookAt for a description
    // of the algorithm. We implement it in a straightforward way:

    // 视线向量
    float fx = center.x - eye.x;
    float fy = center.y - eye.y;
    float fz = center.z - eye.z;

    // Normalize f
    float rlf = 1.0f / sqrt(fx * fx + fy * fy + fz * fz);
    fx *= rlf;
    fy *= rlf;
    fz *= rlf;

    // compute s = f x up (x means "cross product") 侧向向量，垂直于up与视线构成的面
    float sx = fy * up.z - fz * up.y;
    float sy = fz * up.x - fx * up.z;
    float sz = fx * up.y - fy * up.x;

    // and normalize s
    float rls = 1.0f / sqrt(fx * fx + fy * fy + fz * fz);
    sx *= rls;
    sy *= rls;
    sz *= rls;

    // compute u = s x f 重新计算出一个up
    float ux = sy * fz - sz * fy;
    float uy = sz * fx - sx * fz;
    float uz = sx * fy - sy * fx;

    mat[0][0] = sx;
    mat[1][0] = ux;
    mat[2][0] = -fx;
    mat[3][0] = 0.0f;

    mat[0][1] = sy;
    mat[1][1] = uy;
    mat[2][1] = -fy;
    mat[3][1] = 0.0f;

    mat[0][2] = sz;
    mat[1][2] = uz;
    mat[2][2] = -fz;
    mat[3][2] = 0.0f;

    mat[0][3] = 0.0f;
    mat[1][3] = 0.0f;
    mat[2][3] = 0.0f;
    mat[3][3] = 1.0f;

    for (int i=0 ; i<4 ; i++) {
        int mi = i;
        mat[mi][3] += mat[mi][0] * -eye.x + mat[mi][1] * -eye.y + mat[mi][2] * -eye.z;
    }

    return *this;
}

}
