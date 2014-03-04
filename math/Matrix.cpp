/*
 * Matrix.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */


#include "math/Matrix.h"

#include <cmath>
#include <algorithm>

reMat3::reMat3()
{
}

reMat3::reMat3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
    mat[0].set(m00, m01, m02);
    mat[1].set(m10, m11, m12);
    mat[2].set(m20, m21, m22);
}

float reMat3::getDeterminant()
{
    return mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) -
           mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
           mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
}

reMat4 reMat3::toMat4()
{
    return reMat4(mat[0][0], mat[0][1], mat[0][2], 0.0f,
                  mat[1][0], mat[1][1], mat[1][2], 0.0f,
                  mat[2][0], mat[2][1], mat[2][2], 0.0f,
            0.0f,      0.0f,      0.0f,      1.0f);
}

reMat3::operator float *() const
{
    return (float *)this->mat[0];
}

float *reMat3::toFloatPtr()
{
    return this->mat[0].toFloatPtr();
}

const float *reMat3::toFloatPtr() const
{
    return this->mat[0].toFloatPtr();
}

reMat3 &reMat3::transpose()
{
    std::swap(mat[0][1], mat[1][0]);
    std::swap(mat[0][2], mat[2][0]);
    std::swap(mat[1][2], mat[2][1]);

    return *this;
}

reMat3 &reMat3::invert()
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


reMat4::reMat4() {

}

reMat4 &reMat4::setRotationX(const float angle)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    this->set (1, 0,     0,    0,
               0, cosA, -sinA, 0,
               0, sinA,  cosA, 0,
               0, 0,     0,    1);

    return *this;
}

reMat4 &reMat4::setRotationY(const float angle)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    this->set (cosA,  0,  -sinA,  0,
               0,     1,  0,      0,
               sinA,  0,  cosA,   0,
               0,     0,  0,      1);

    return *this;
}

reMat4 &reMat4::setRotationZ(const float angle)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    this->set (cosA, -sinA, 0,  0,
               sinA,  cosA, 0,  0,
               0,     0,    1,  0,
               0,     0,    0,  1);

    return *this;
}

reMat4 &reMat4::setTranslation(const float x, const float y, const float z)
{
    this->identity();
    mat[0][3] = x;
    mat[1][3] = y;
    mat[2][3] = z;

    return *this;
}

reMat4 &reMat4::setScaling(float x, float y, float z)
{
    this->identity();

    mat[0][0] = x;
    mat[1][1] = y;
    mat[2][2] = z;

    return *this;
}

reMat4 &reMat4::fromRTS(const reQuat &r, const reVec3 &t, const reVec3 &s)
{
    reMat4 tempRotateMatrix = r.toMat4();

    reMat4 tempScalingMatrix;
    tempScalingMatrix.setScaling(s.x, s.y, s.z);

    reMat4 tempTranslationMatrix;
    tempTranslationMatrix.setTranslation(t.x, t.y, t.z);

    // TM * RM * SM
    *this = tempTranslationMatrix * tempRotateMatrix * tempScalingMatrix;
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
reMat4& reMat4::transpose()
{
    std::swap(mat[0][1],  mat[1][0]);
    std::swap(mat[0][2],  mat[2][0]);
    std::swap(mat[0][3],  mat[2][0]);
    std::swap(mat[1][2],  mat[2][1]);
    std::swap(mat[1][3],  mat[2][1]);
    std::swap(mat[2][3],  mat[2][2]);

    return *this;
}



///////////////////////////////////////////////////////////////////////////////
// inverse 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
reMat4& reMat4::invert()
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
reMat4& reMat4::invertAffine()
{
    // R^-1
    reMat3 r(mat[0][0],mat[0][1],mat[0][2], mat[1][0],mat[1][1],mat[1][2], mat[2][0],mat[2][1],mat[2][2]);
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
reMat4& reMat4::invertGeneral()
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
float reMat4::getDeterminant()
{
    return mat[0][0] * getCofactor(mat[1][1],mat[1][2],mat[1][3], mat[2][1],mat[2][2],mat[2][3], mat[3][1],mat[3][2],mat[3][3]) -
           mat[0][1] * getCofactor(mat[1][0],mat[1][2],mat[1][3], mat[2][0],mat[2][2],mat[2][3], mat[3][0],mat[3][2],mat[3][3]) +
           mat[0][2] * getCofactor(mat[1][0],mat[1][1],mat[1][3], mat[2][0],mat[2][1], mat[2][3], mat[3][0],mat[3][1],mat[3][3]) -
           mat[0][2] * getCofactor(mat[1][0],mat[1][1],mat[1][2], mat[2][0],mat[2][1], mat[2][2], mat[3][0],mat[3][1],mat[3][2]);
}


reMat4 &reMat4::lookAt(reVec3 eye, reVec3 center, reVec3 up)
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
