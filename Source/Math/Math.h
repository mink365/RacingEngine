#ifndef MATHTOOL_H
#define MATHTOOL_H

#include <cmath>
#include <cstdlib>

#include "Vector.h"
#include "Ray.h"
#include "Plane.h"
#include "AABBox.h"
#include "OBBox.h"
#include "Sphere.h"
#include "Frustum.h"

namespace re {

const static float  PI				= 3.14159265358979323846f;
const static float	TWO_PI			= 2.0f * PI;
const static float	HALF_PI			= 0.5f * PI;
const static float	ONEFOURTH_PI	= 0.25f * PI;
const static float  E				= 2.71828182845904523536f;
const static float  DEG_TO_RAD      = PI / 180.0f;
const static float  RAD_TO_DEG      = 180.0f / PI;

inline float DegreeToRadian (const float d) {
    return d * DEG_TO_RAD;
}

inline float RadianToDegree(const float r) {
    return r * RAD_TO_DEG;
}

inline float Fabs( float f ) {
    int tmp = *reinterpret_cast<int *>( &f );
    tmp &= 0x7FFFFFFF;
    return *reinterpret_cast<float *>( &tmp );
}

inline int CeilPowerOfTwo( int x ) {
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

inline int FloorPowerOfTwo( int x ) {
    return CeilPowerOfTwo( x ) >> 1;
}

inline bool IsPowerOfTwo( int x ) {
    return ( x & ( x - 1 ) ) == 0 && x > 0;
}

inline bool InBound(float p, float min, float max)
{
    return p >= min && p <= max;
}

    enum BoundOverlap
    {
        BO_Yes,
        BO_No,
        BO_Partial
    };


    namespace MathLib
    {
        inline float abs(float v) {
            return Fabs(v);
        }

//        float abs(float x);
        float sqrt(float x);
        float recip_sqrt(float number);

        float pow(float x, float y);
        float exp(float x);

        float log(float x);
        float log10(float x);

        float sin(float x);
        float cos(float x);
        void sincos(float x, float& s, float& c);
        float tan(float x);

        float asin(float x);
        float acos(float x);
        float atan(float x);

        float sinh(float x);
        float cosh(float x);
        float tanh(float x);

        inline Vec3 abs(const Vec3& v) {
            return Vec3(MathLib::abs(v.x), MathLib::abs(v.y), MathLib::abs(v.z));
        }

        inline float dot(const Vec3& lhs, const Vec3& rhs){
            return lhs | rhs;
        }

        inline float length_sq(const Vec3& lhs) {
            return lhs.lengthSqr();
        }

        inline float Rand() {
            return rand() / (float)RAND_MAX;
        }

        inline float sign(float v) {
            return v >= 0 ? 1 : -1;
        }

        template<typename T>
        T lerp(const T& lhs, const T& rhs, float s);

        template <typename T>
        T maximize(T const & lhs, T const & rhs);

        template <typename T>
        T minimize(T const & lhs, T const & rhs);

        template <typename T>
        T normalize(const T& rhs);

        Plane mul(Plane const & p, Mat4 const & mat);

        void decompose(const Mat4 & mat, Vec3& scale, Quat& rot, Vec3& trans);

        Mat4 transformation(Vec3 const * scaling_center, Quat const * scaling_rotation, Vec3 const * scale,
            Vec3 const * rotation_center, Quat const * rotation, Vec3 const * trans);

        Vec3 transform_quat(Vec3 const & v, Quat const & quat);
        Vec3 transform_coord(const Vec3& v, const Mat4& mat);

        AABBox ConvertToAABBox(OBBox const & obb);
        OBBox ConvertToOBBox(AABBox const & aabb);

        AABBox TransformAABB(const AABBox& aabb, const Mat4& mat);
        OBBox TransformOBB(const OBBox& obb, const Mat4& mat);
        Sphere TransformSphere(const Sphere& sphere, const Mat4& mat);
        Frustum TransformFrustum(const Frustum& frustum, const Mat4& mat);

        AABBox TransformAABB(const AABBox& aabb, const Vec3& scale, const Quat& rot, const Vec3& trans);
        OBBox TransformOBB(const OBBox& obb, const Vec3& scale, const Quat& rot, const Vec3& trans);
        Sphere TransformSphere(const Sphere& sphere, float scale, const Quat& rot, const Vec3& trans);
        Frustum TransformFrustum(const Frustum& frustum, float scale, const Quat& rot, const Vec3& trans);

        bool IntersectPointAABB(const Vec3& p, const AABBox& aabb);
        bool IntersectPointOBB(const Vec3& p, const OBBox& obb);
        bool IntersectPointSphere(const Vec3& p, const Sphere& sphere);
        bool IntersectPointFrustum(const Vec3& p, const Frustum& frustum);

        bool IntersectRayAABB(const Ray& ray, const AABBox& aabb);
        bool IntersectRayOBB(const Ray& ray, const OBBox& obb);
        bool IntersectRaySphere(const Ray& ray, const Sphere& sphere);

        bool IntersectAABBoxAABBox(const AABBox& lhs, const AABBox& aabb);
        bool IntersectAABBoxOBBox(const AABBox& aabb, const OBBox& obb);
        bool IntersectAABBoxSphere(const AABBox& aabb, const Sphere& sphere);

        bool IntersectOBBoxOBBox(const OBBox& lhs, const OBBox& obb);
        bool IntersectOBBoxSphere(const OBBox& obb, const Sphere& sphere);

        bool IntersectSphereSphere(const Sphere& lhs, const Sphere& sphere);

        BoundOverlap IntersectAABBoxFrustum(const AABBox& aabb, const Frustum& frustum);
        BoundOverlap IntersectOBBoxFrustum(const OBBox& obb, const Frustum& frustum);
        BoundOverlap IntersectSphereFrustum(const Sphere& sphere, const Frustum& frustum);
        BoundOverlap IntersectFrustumFrustum(const Frustum& lhs, const Frustum& frustum);
    }

}

#endif // MATHTOOL_H
