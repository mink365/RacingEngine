/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "Math.h"
#include "Vector.h"
#include "Color.h"
#include "UV.h"
#include "Matrix.h"

#include "MathUtil.h"

namespace re
{

namespace MathLib
{
    float sqrt(float x)
    {
        return std::sqrt(x);
    }

    float recip_sqrt(float number)
    {
        float const threehalfs = 1.5f;

        float x2 = number * 0.5f;
        union FNI
        {
            float f;
            int32_t i;
        } fni;
        fni.f = number;											// evil floating point bit level hacking
        fni.i = 0x5f375a86 - (fni.i >> 1);						// what the fuck?
        fni.f = fni.f * (threehalfs - (x2 * fni.f * fni.f));	// 1st iteration
        fni.f = fni.f * (threehalfs - (x2 * fni.f * fni.f));		// 2nd iteration, this can be removed

        return fni.f;
    }

    float pow(float x, float y)
    {
        return std::pow(x, y);
    }

    float exp(float x)
    {
        return std::exp(x);
    }

    float log(float x)
    {
        return std::log(x);
    }

    float log10(float x)
    {
        return std::log10(x);
    }

    float sin(float x)
    {
        return std::sin(x);
    }

    float cos(float x)
    {
        return sin(x + PI / 2);
    }

    void sincos(float x, float& s, float& c)
    {
        s = sin(x);
        c = cos(x);
    }

    float tan(float x)
    {
        return std::tan(x);
    }

    float asin(float x)
    {
        return std::asin(x);
    }

    float acos(float x)
    {
        return std::acos(x);
    }

    float atan(float x)
    {
        return std::atan(x);
    }

    float sinh(float x)
    {
        return std::sinh(x);
    }

    float cosh(float x)
    {
        return std::cosh(x);
    }

    float tanh(float x)
    {
        return std::tanh(x);
    }

    float Dot(const Plane& lhs, const Vec4& rhs)
    {
        return lhs.a() * rhs.x + lhs.b() * rhs.y + lhs.c() * rhs.z + lhs.d() * rhs.w;
    }

    float DotCoord(const Plane& lhs, const Vec3& rhs)
    {
        return (lhs.Normal() | rhs) + lhs.d();
    }

    float DotNormal(const Plane& lhs, const Vec3& rhs)
    {
        return lhs.Normal() | rhs;
    }

    float dot_coord(const Plane& lhs, const Vec3& rhs)
    {
        return DotCoord(lhs, rhs);
    }

    Vec3 cross(const Vec3& lhs, const Vec3& rhs)
    {
        return lhs.cross(rhs);
    }

    Mat4 translation(const Vec3& trans)
    {
        Mat4 result;

        return result.setTranslation(trans);
    }

    Mat4 to_matrix(const Quat& rot)
    {
        return rot.toMat4();
    }

    Mat4 scaling(const Vec3& scale)
    {
        Mat4 result;

        return result.setScaling(scale);
    }

    Mat4 inverse(const Mat4& rhs)
    {
        return rhs.inverse();
    }

    template float lerp(const float& lhs, const float& rhs, float s);
    template Vec2 lerp(const Vec2& lhs, const Vec2& rhs, float s);
    template Vec3 lerp(const Vec3& lhs, const Vec3& rhs, float s);
    template Vec4 lerp(const Vec4& lhs, const Vec4& rhs, float s);
    template Color lerp(const Color& lhs, const Color& rhs, float s);
    template Uv lerp(const Uv& lhs, const Uv& rhs, float s);

    template <typename T>
    T lerp(const T& lhs, const T& rhs, float s)
    {
        return lhs + (rhs - rhs) * s;
    }

    template Vec2 minimize(const Vec2& lhs, const Vec2& rhs);
    template Vec3 minimize(const Vec3& lhs, const Vec3& rhs);
    template Vec4 minimize(const Vec4& lhs, const Vec4& rhs);

    template <typename T>
    T minimize(const T &lhs, const T &rhs)
    {
        T ret;
        max_minimize_helper<float, T::elem_num>::DoMin(lhs, rhs, ret);

        return ret;
    }

    template Vec2 maximize(const Vec2& lhs, const Vec2& rhs);
    template Vec3 maximize(const Vec3& lhs, const Vec3& rhs);
    template Vec4 maximize(const Vec4& lhs, const Vec4& rhs);

    template <typename T>
    T maximize(const T &lhs, const T &rhs)
    {
        T ret;
        max_minimize_helper<float, T::elem_num>::DoMax(lhs, rhs, ret);

        return ret;
    }

    template <typename T>
    T normalize(const T& rhs)
    {
        return rhs.normalize();
    }

    Plane mul(const Plane &p, const Mat4 &mat)
    {
        return Plane(
            p.a() * mat(0, 0) + p.b() * mat(1, 0) + p.c() * mat(2, 0) + p.d() * mat(3, 0),
            p.a() * mat(0, 1) + p.b() * mat(1, 1) + p.c() * mat(2, 1) + p.d() * mat(3, 1),
            p.a() * mat(0, 2) + p.b() * mat(1, 2) + p.c() * mat(2, 2) + p.d() * mat(3, 2),
            p.a() * mat(0, 3) + p.b() * mat(1, 3) + p.c() * mat(2, 3) + p.d() * mat(3, 3));
    }

    void decompose(const Mat4 &rhs, Vec3 &scale, Quat &rot, Vec3 &trans)
    {
        scale.x = sqrt(rhs(0, 0) * rhs(0, 0) + rhs(0, 1) * rhs(0, 1) + rhs(0, 2) * rhs(0, 2));
        scale.y = sqrt(rhs(1, 0) * rhs(1, 0) + rhs(1, 1) * rhs(1, 1) + rhs(1, 2) * rhs(1, 2));
        scale.z = sqrt(rhs(2, 0) * rhs(2, 0) + rhs(2, 1) * rhs(2, 1) + rhs(2, 2) * rhs(2, 2));

        trans = Vec3(rhs(3, 0), rhs(3, 1), rhs(3, 2));

        Mat4 rot_mat;
        rot_mat(0, 0) = rhs(0, 0) / scale.x;
        rot_mat(0, 1) = rhs(0, 1) / scale.x;
        rot_mat(0, 2) = rhs(0, 2) / scale.x;
        rot_mat(0, 3) = 0;
        rot_mat(1, 0) = rhs(1, 0) / scale.y;
        rot_mat(1, 1) = rhs(1, 1) / scale.y;
        rot_mat(1, 2) = rhs(1, 2) / scale.y;
        rot_mat(1, 3) = 0;
        rot_mat(2, 0) = rhs(2, 0) / scale.z;
        rot_mat(2, 1) = rhs(2, 1) / scale.z;
        rot_mat(2, 2) = rhs(2, 2) / scale.z;
        rot_mat(2, 3) = 0;
        rot_mat(3, 0) = 0;
        rot_mat(3, 1) = 0;
        rot_mat(3, 2) = 0;
        rot_mat(3, 3) = 1;

//        rot = to_quaternion(rot_mat);
        rot.fromMatrix(rot_mat);
    }

    Mat4 transformation(const Vec3 *scaling_center, const Quat *scaling_rotation, const Vec3 *scale, const Vec3 *rotation_center, const Quat *rotation, const Vec3 *trans)
    {
        Vec3 psc, prc, pt;
        if (!scaling_center)
        {
            psc = Vec3(float(0), float(0), float(0));
        }
        else
        {
            psc = *scaling_center;
        }
        if (!rotation_center)
        {
            prc = Vec3(float(0), float(0), float(0));
        }
        else
        {
            prc = *rotation_center;
        }
        if (!trans)
        {
            pt = Vec3(float(0), float(0), float(0));
        }
        else
        {
            pt = *trans;
        }

        Mat4 m1, m2, m3, m4, m5, m6, m7;
        m1 = translation(-psc);
        if (!scaling_rotation)
        {
            m2 = m4 = Mat4::Identity;
        }
        else
        {
            m4 = to_matrix(*scaling_rotation);
            m2 = inverse(m4);
        }
        if (!scale)
        {
            m3 = Mat4::Identity;
        }
        else
        {
            m3 = scaling(*scale);
        }
        if (!rotation)
        {
            m6 = Mat4::Identity;
        }
        else
        {
            m6 = to_matrix(*rotation);
        }
        m5 = translation(psc - prc);
        m7 = translation(prc + pt);

        return m1 * m2 * m3 * m4 * m5 * m6 * m7;
    }

    Vec3 transform_quat(const Vec3 &v, const Quat &quat)
    {
        return v + cross(quat.v(), cross(quat.v(), v) + quat.w * v) * float(2);
    }

    Vec3 transform_coord(const Vec3 &v, const Mat4 &mat)
    {
        return mat * v;
    }

    AABBox TransformAABB(const AABBox &aabb, const Mat4 &mat)
    {
        Vec3 scale, trans;
        Quat rot;

        decompose(mat, scale, rot, trans);

        return TransformAABB(aabb, scale, rot, trans);
    }

    OBBox TransformOBB(const OBBox &obb, const Mat4 &mat)
    {
        Vec3 scale, trans;
        Quat rot;

        decompose(mat, scale, rot, trans);

        return TransformOBB(obb, scale, rot, trans);
    }

    Sphere TransformSphere(const Sphere &sphere, const Mat4 &mat)
    {
        Vec3 scale, trans;
        Quat rot;

        decompose(mat, scale, rot, trans);

        return TransformSphere(sphere, scale.x, rot, trans);
    }

    Frustum TransformFrustum(const Frustum &frustum, const Mat4 &mat)
    {
        Frustum ret;
        for (int i = 0; i < 6; ++ i)
        {
            ret.FrustumPlane(i, normalize(mul(frustum.FrustumPlane(i), mat)));
        }
        for (int i = 0; i < 8; ++ i)
        {
            ret.Corner(i, transform_coord(frustum.Corner(i), mat));
        }

        return ret;
    }

    AABBox TransformAABB(const AABBox &aabb, const Vec3 &scale, const Quat &rot, const Vec3 &trans)
    {
        Vec3 min, max;
        min = max = transform_quat(aabb.Corner(0) * scale, rot) + trans;
        for (size_t i = 1; i < 8; ++i) {
            Vec3 v = transform_quat(aabb.Corner(i) * scale, rot) + trans;

            min = minimize(min, v);
            max = maximize(max, v);
        }

        return AABBox(min, max);
    }

    OBBox TransformOBB(const OBBox &obb, const Vec3 &scale, const Quat &rot, const Vec3 &trans)
    {
        Vec3 center = transform_quat(obb.Center() * scale, rot) + trans;
        Quat rotation = obb.Rotation() * rot;
        Vec3 extent = obb.HalfSize() * scale;

        return OBBox(center, rotation, extent);
    }

    Sphere TransformSphere(const Sphere &sphere, float scale, const Quat &rot, const Vec3 &trans)
    {
        Vec3 center = transform_quat(sphere.Center() * scale, rot) + trans;
        float radius = sphere.Radius() * scale;

        return Sphere(center, radius);
    }

    Frustum TransformFrustum(const Frustum &frustum, float scale, const Quat &rot, const Vec3 &trans)
    {
        Vec3 vscale(scale, scale, scale);
        return TransformFrustum(frustum, transformation(nullptr, nullptr, &vscale, nullptr, &rot, &trans));
    }
}

AABBox MathLib::ConvertToAABBox(const OBBox &obb)
{
    Vec3 min(+1e10f, +1e10f, +1e10f);
    Vec3 max(-1e10f, -1e10f, -1e10f);

    Vec3 const & center = obb.Center();
    Vec3 const & extent = obb.HalfSize();
    Vec3 const extent_x = extent.x * obb.Axis(0);
    Vec3 const extent_y = extent.y * obb.Axis(1);
    Vec3 const extent_z = extent.z * obb.Axis(2);
    for (int i = 0; i < 8; ++ i)
    {
        Vec3 corner = center + ((i & 1) ? extent_x : -extent_x)
            + ((i & 2) ? extent_y : -extent_y) + ((i & 4) ? extent_z : -extent_z);

        min = minimize(min, corner);
        max = maximize(max, corner);
    }

    return AABBox(min, max);
}

OBBox MathLib::ConvertToOBBox(const AABBox &aabb)
{
    return OBBox(aabb.Center(), Quat::Identity(), aabb.HalfSize());
}

bool MathLib::IntersectPointAABB(const Vec3& p, const AABBox& aabb)
{
    return ((InBound(p.x, aabb.Min().x, aabb.Max().x))
            && (InBound(p.y, aabb.Min().y, aabb.Max().y))
            && (InBound(p.z, aabb.Min().z, aabb.Max().z)));
}

bool MathLib::IntersectPointOBB(const Vec3& p, const OBBox& obb)
{
    Vec3 d = p - obb.Center();

    return ((dot(obb.Axis(0), d) <= obb.HalfSize().x)
            && (dot(obb.Axis(1), d) <= obb.HalfSize().y)
            && (dot(obb.Axis(2), d) <= obb.HalfSize().z));
}

bool MathLib::IntersectPointSphere(const Vec3& p, const Sphere& sphere)
{
    Vec3 v = p - sphere.Center();

    return (v.length() < sphere.Radius());
}

bool MathLib::IntersectPointFrustum(const Vec3& p, const Frustum& frustum)
{
    for (int i = 0; i < 6; ++i)
    {
        if (DotCoord(frustum.FrustumPlane(i), p) < 0) {
            return false;
        }
    }

    return true;
}

bool MathLib::IntersectRayAABB(const Ray& ray, const AABBox& aabb)
{
    float near = -1e10;
    float far = +1e10;

    for (int i = 0; i < 3; ++i) {
        if (ray.Direction()[i] == 0)
        {
            if (ray.Direction()[i] < aabb.Min()[i] || ray.Direction()[i] > aabb.Max()[i]) {
                return false;
            }
        }
        else
        {
            float t1 = (aabb.Min()[i] - ray.Origin()[i]) / ray.Direction()[i];
            float t2 = (aabb.Max()[i] - ray.Origin()[i]) / ray.Direction()[i];

            if (t1 < t2) {
                std::swap(t1, t2);
            }

            if (t1 > near) {
                near = t1;
            }
            if (t2 < far) {
                far = t2;
            }

            if (near < far) {
                // box is missed
                return false;
            }
            if (far < 0) {
                // box is behind ray
                return false;
            }
        }
    }

    return true;
}

bool MathLib::IntersectRayOBB(const Ray& ray, const OBBox& obb)
{
    float t_near = float(-1e10);
    float t_far = float(+1e10);

    Vec3 p = obb.Center() - ray.Origin();
    Vec3 const & extent = obb.HalfSize();
    for (int i = 0; i < 3; ++ i)
    {
        float e = dot(obb.Axis(i), p);
        float f = dot(obb.Axis(i), ray.Direction());
        if (f == 0)
        {
            if ((e < -extent[i]) || (e > extent[i]))
            {
                return false;
            }
        }
        else
        {
            float t1 = (e + extent[i]) / f;
            float t2 = (e - extent[i]) / f;
            if (t1 > t2)
            {
                std::swap(t1, t2);
            }
            if (t1 > t_near)
            {
                t_near = t1;
            }
            if (t2 < t_far)
            {
                t_far = t2;
            }

            if (t_near > t_far)
            {
                // box is missed
                return false;
            }
            if (t_far < 0)
            {
                // box is behind ray
                return false;
            }
        }
    }

    return true;
}

bool MathLib::IntersectRaySphere(const Ray& ray, const Sphere& sphere)
{
    float const a = ray.Direction().lengthSqr();
    float const b = 2 * dot(ray.Direction(), (ray.Origin() - sphere.Center()));
    float const c = (ray.Origin() - sphere.Center()).lengthSqr() - sphere.Radius() * sphere.Radius();

    if (b * b - 4 * a * c < 0)
    {
        return false;
    }
    return true;
}

bool MathLib::IntersectAABBoxAABBox(const AABBox& lhs, const AABBox& aabb)
{
    Vec3 const t = aabb.Center() - lhs.Center();
    Vec3 const e = aabb.HalfSize() + lhs.HalfSize();
    return (MathLib::abs(t.x) <= e.x) && (MathLib::abs(t.y) <= e.y) && (MathLib::abs(t.z) <= e.z);
}

bool MathLib::IntersectAABBoxOBBox(const AABBox& aabb, const OBBox& obb)
{
    return IntersectOBBoxOBBox(ConvertToOBBox(aabb), obb);
}

bool MathLib::IntersectAABBoxSphere(const AABBox& lhs, const Sphere& sphere)
{
    Vec3 half_size = lhs.HalfSize();
    Vec3 d = sphere.Center() - lhs.Center();
    Vec3 closest_point_on_obb = lhs.Center();
    for (int i = 0; i < 3; ++ i)
    {
        Vec3 axis(0, 0, 0);
        axis[i] = 1;
        float dist = dot(axis, d);
        if (dist > half_size[i])
        {
            dist = half_size[i];
        }
        if (dist < -half_size[i])
        {
            dist = -half_size[i];
        }
        closest_point_on_obb += dist * axis;
    }

    Vec3 v = closest_point_on_obb - sphere.Center();
    return v.lengthSqr() <= sphere.Radius() * sphere.Radius();
}

bool MathLib::IntersectOBBoxOBBox(const OBBox& lhs, const OBBox& obb)
{
    // From Real-Time Collision Detection, p. 101-106. See http://realtimecollisiondetection.net/

    float epsilon = float(1e-3);

    Mat4 r_mat = Mat4::Identity;
    for (int i = 0; i < 3; ++ i)
    {
        for (int j = 0; j < 3; ++ j)
        {
            r_mat(i, j) = dot(lhs.Axis(i), obb.Axis(j));
        }
    }

    Vec3 t = obb.Center() - lhs.Center();
    t = Vec3(dot(t, lhs.Axis(0)), dot(t, lhs.Axis(1)), dot(t, lhs.Axis(2)));

    Mat4 abs_r_mat = Mat4::Identity;
    for (int i = 0; i < 3; ++ i)
    {
        for (int j = 0; j < 3; ++ j)
        {
            abs_r_mat(i, j) = MathLib::abs(r_mat(i, j)) + epsilon;
        }
    }

    Vec3 const & lr = lhs.HalfSize();
    Vec3 const & rr = obb.HalfSize();

    // Test the three major axes of this OBB.
    for (int i = 0; i < 3; ++ i)
    {
        float ra = lr[i];
        float rb = rr[0] * abs_r_mat(i, 0) +  rr[1] * abs_r_mat(i, 1) + rr[2] * abs_r_mat(i, 2);
        if (MathLib::abs(t[i]) > ra + rb)
        {
            return false;
        }
    }

    // Test the three major axes of the OBB b.
    for (int i = 0; i < 3; ++ i)
    {
        float ra = lr[0] * abs_r_mat(0, i) + lr[1] * abs_r_mat(1, i) + lr[2] * abs_r_mat(2, i);
        float rb = rr[i];
        if (MathLib::abs(t.x * r_mat(0, i) + t.y * r_mat(1, i) + t.z * r_mat(2, i)) > ra + rb)
        {
            return false;
        }
    }

    // Test the 9 different cross-axes.

    // A.x <cross> B.x
    float ra = lr.y * abs_r_mat(2, 0) + lr.z * abs_r_mat(1, 0);
    float rb = rr.y * abs_r_mat(0, 2) + rr.z * abs_r_mat(0, 1);
    if (MathLib::abs(t.z * r_mat(1, 0) - t.y * r_mat(2, 0)) > ra + rb)
    {
        return false;
    }

    // A.x < cross> B.y
    ra = lr.y * abs_r_mat(2, 1) + lr.z * abs_r_mat(1, 1);
    rb = rr.x * abs_r_mat(0, 2) + rr.z * abs_r_mat(0, 0);
    if (MathLib::abs(t.z * r_mat(1, 1) - t.y * r_mat(2, 1)) > ra + rb)
    {
        return false;
    }

    // A.x <cross> B.z
    ra = lr.y * abs_r_mat(2, 2) + lr.z * abs_r_mat(1, 2);
    rb = rr.x * abs_r_mat(0, 1) + rr.y * abs_r_mat(0, 0);
    if (MathLib::abs(t.z * r_mat(1, 2) - t.y * r_mat(2, 2)) > ra + rb)
    {
        return false;
    }

    // A.y <cross> B.x
    ra = lr.x * abs_r_mat(2, 0) + lr.z * abs_r_mat(0, 0);
    rb = rr.y * abs_r_mat(1, 2) + rr.z * abs_r_mat(1, 1);
    if (MathLib::abs(t.x * r_mat(2, 0) - t.z * r_mat(0, 0)) > ra + rb)
    {
        return false;
    }

    // A.y <cross> B.y
    ra = lr.x * abs_r_mat(2, 1) + lr.z * abs_r_mat(0, 1);
    rb = rr.x * abs_r_mat(1, 2) + rr.z * abs_r_mat(1, 0);
    if (MathLib::abs(t.x * r_mat(2, 1) - t.z * r_mat(0, 1)) > ra + rb)
    {
        return false;
    }

    // A.y <cross> B.z
    ra = lr.x * abs_r_mat(2, 2) + lr.z * abs_r_mat(0, 2);
    rb = rr.x * abs_r_mat(1, 1) + rr.y * abs_r_mat(1, 0);
    if (MathLib::abs(t.x * r_mat(2, 2) - t.z * r_mat(0, 2)) > ra + rb)
    {
        return false;
    }

    // A.z <cross> B.x
    ra = lr.x * abs_r_mat(1, 0) + lr.y * abs_r_mat(0, 0);
    rb = rr.y * abs_r_mat(2, 2) + rr.z * abs_r_mat(2, 1);
    if (MathLib::abs(t.y * r_mat(0, 0) - t.x * r_mat(1, 0)) > ra + rb)
    {
        return false;
    }

    // A.z <cross> B.y
    ra = lr.x * abs_r_mat(1, 1) + lr.y * abs_r_mat(0, 1);
    rb = rr.x * abs_r_mat(2, 2) + rr.z * abs_r_mat(2, 0);
    if (MathLib::abs(t.y * r_mat(0, 1) - t.x * r_mat(1, 1)) > ra + rb)
    {
        return false;
    }

    // A.z <cross> B.z
    ra = lr.x * abs_r_mat(1, 2) + lr.y * abs_r_mat(0, 2);
    rb = rr.x * abs_r_mat(2, 1) + rr.y * abs_r_mat(2, 0);
    if (MathLib::abs(t.y * r_mat(0, 2) - t.x * r_mat(1, 2)) > ra + rb)
    {
        return false;
    }

    return true;
}

bool MathLib::IntersectOBBoxSphere(const OBBox& lhs, const Sphere& sphere)
{
    Vec3 d = sphere.Center() - lhs.Center();
    Vec3 closest_point_on_obb = lhs.Center();
    for (int i = 0; i < 3; ++ i)
    {
        float dist = dot(d, lhs.Axis(i));
        if (dist > lhs.HalfSize()[i])
        {
            dist = lhs.HalfSize()[i];
        }
        if (dist < -lhs.HalfSize()[i])
        {
            dist = -lhs.HalfSize()[i];
        }
        closest_point_on_obb += dist * lhs.Axis(i);
    }

    Vec3 v = closest_point_on_obb - sphere.Center();
    return length_sq(v) <= sphere.Radius() * sphere.Radius();
}

bool MathLib::IntersectSphereSphere(const Sphere& lhs, const Sphere& sphere)
{
    Vec3 d = lhs.Center() - sphere.Center();
    float r = lhs.Radius() + sphere.Radius();
    return length_sq(d) <= r * r;
}

BoundOverlap MathLib::IntersectAABBoxFrustum(const AABBox& aabb, const Frustum& frustum)
{
    Vec3 const & min_pt = aabb.Min();
    Vec3 const & max_pt = aabb.Max();

    bool intersect = false;
    for (int i = 0; i < 6; ++ i)
    {
        Plane const & plane = frustum.FrustumPlane(i);

        // v1 is diagonally opposed to v0
        Vec3 v0((plane.a() < 0) ? min_pt.x : max_pt.x, (plane.b() < 0) ? min_pt.y : max_pt.y, (plane.c() < 0) ? min_pt.z : max_pt.z);
        Vec3 v1((plane.a() < 0) ? max_pt.x : min_pt.x, (plane.b() < 0) ? max_pt.y : min_pt.y, (plane.c() < 0) ? max_pt.z : min_pt.z);

        if (dot_coord(plane, v0) < 0)
        {
            return BO_No;
        }
        if (dot_coord(plane, v1) < 0)
        {
            intersect = true;
        }
    }

    return intersect ? BO_Partial : BO_Yes;
}

BoundOverlap MathLib::IntersectOBBoxFrustum(const OBBox& obb, const Frustum& frustum)
{
    Vec3 min_pt = obb.Corner(0);
    Vec3 max_pt = min_pt;
    for (int i = 1; i < 8; ++ i)
    {
        Vec3 corner = obb.Corner(i);

        min_pt = minimize(min_pt, corner);
        max_pt = maximize(max_pt, corner);
    }

    bool intersect = false;
    for (int i = 0; i < 6; ++ i)
    {
        Plane const & plane = frustum.FrustumPlane(i);

        // v1 is diagonally opposed to v0
        Vec3 v0((plane.a() < 0) ? min_pt.x : max_pt.x, (plane.b() < 0) ? min_pt.y : max_pt.y, (plane.c() < 0) ? min_pt.z : max_pt.z);
        Vec3 v1((plane.a() < 0) ? max_pt.x : min_pt.x, (plane.b() < 0) ? max_pt.y : min_pt.y, (plane.c() < 0) ? max_pt.z : min_pt.z);

        if (dot_coord(plane, v0) < 0)
        {
            return BO_No;
        }
        if (dot_coord(plane, v1) < 0)
        {
            intersect = true;
        }
    }

    return intersect ? BO_Partial : BO_Yes;
}

BoundOverlap MathLib::IntersectSphereFrustum(const Sphere& sphere, const Frustum& frustum)
{
    bool intersect = false;
    for (int i = 0; i < 6; ++ i)
    {
        Plane const & plane = frustum.FrustumPlane(i);

        float d = dot_coord(plane, sphere.Center());
        if (d <= -sphere.Radius())
        {
            return BO_No;
        }
        if (d > sphere.Radius())
        {
            intersect = true;
        }
    }

    return intersect ? BO_Partial : BO_Yes;
}

BoundOverlap MathLib::IntersectFrustumFrustum(const Frustum& lhs, const Frustum& rhs)
{
    bool outside = false;
    bool inside_all = true;
    for (int i = 0; i < 6; ++ i)
    {
        Plane const & p = lhs.FrustumPlane(i);

        float min_p, max_p;
        min_p = max_p = dot_coord(p, rhs.Corner(0));
        for (int j = 1; j < 8; ++ j)
        {
            float tmp = dot_coord(p, rhs.Corner(j));
            min_p = std::min(min_p, tmp);
            max_p = std::max(max_p, tmp);
        }

        outside |= (min_p > 0);
        inside_all &= (max_p <= 0);
    }
    if (outside)
    {
        return BO_No;
    }
    if (inside_all)
    {
        return BO_Yes;
    }

    for (int i = 0; i < 6; ++ i)
    {
        Plane const & p = rhs.FrustumPlane(i);

        float min_p = dot_coord(p, lhs.Corner(0));
        for (int j = 1; j < 8; ++ j)
        {
            float tmp = dot_coord(p, lhs.Corner(j));
            min_p = std::min(min_p, tmp);
        }

        outside |= (min_p > 0);
    }
    if (outside)
    {
        return BO_No;
    }

    Vec3 edge_axis_l[6];
    edge_axis_l[0] = rhs.Corner(6);
    edge_axis_l[1] = rhs.Corner(4);
    edge_axis_l[2] = rhs.Corner(5);
    edge_axis_l[3] = rhs.Corner(7);
    edge_axis_l[4] = rhs.Corner(6) - rhs.Corner(5);
    edge_axis_l[5] = rhs.Corner(7) - rhs.Corner(5);

    Vec3 edge_axis_r[6];
    edge_axis_r[0] = lhs.Corner(6);
    edge_axis_r[1] = lhs.Corner(4);
    edge_axis_r[2] = lhs.Corner(5);
    edge_axis_r[3] = lhs.Corner(7);
    edge_axis_r[4] = lhs.Corner(6) - lhs.Corner(5);
    edge_axis_r[5] = lhs.Corner(7) - lhs.Corner(5);

    for (int i = 0; i < 6; ++ i)
    {
        for (int j = 0; j < 6; ++ j)
        {
            Vec3 Axis = cross(edge_axis_l[i], edge_axis_r[j]);

            float min_l, max_l, min_r, max_r;
            min_l = max_l = dot(Axis, rhs.Corner(0));
            min_r = max_r = dot(Axis, lhs.Corner(0));
            for (int k = 1; k < 8; ++ k)
            {
                float tmp = dot(Axis, rhs.Corner(k));
                min_l = std::min(min_l, tmp);
                max_l = std::max(max_l, tmp);

                tmp = dot(Axis, lhs.Corner(k));
                min_r = std::min(min_r, tmp);
                max_r = std::max(max_r, tmp);
            }

            outside |= min_l > max_r;
            outside |= min_r > max_l;
        }
    }
    if (outside)
    {
        return BO_No;
    }

    return BO_Partial;
}

}
