#ifndef MATHTOOL_H
#define MATHTOOL_H

#include <random>

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

        inline Vec3 absVec3(const Vec3& v) {
            return Vec3(MathLib::abs(v.x), MathLib::abs(v.y), MathLib::abs(v.z));
        }

        inline float dot(const Vec3& lhs, const Vec3& rhs){
            return lhs * rhs;
        }

        inline float length_sq(const Vec3& lhs) {
            return lhs.lengthSqr();
        }

        inline float Rand() {
            return std::rand() / (float)RAND_MAX;
        }

        AABBox ConvertToAABBox(OBBox const & obb);
        OBBox ConvertToOBBox(AABBox const & aabb);

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
