/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_TRANSFORM_H
#define RE_TRANSFORM_H

#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Math/Matrix.h"
#include "Base/ECS/Component.h"

namespace re {

class Transform;
typedef ComponentHandle<Transform> TransformPtr;

class Transform : public Component<Transform>
{
    friend class SceneManager;
    template<class T>
    friend class Component;

public:
    Transform();
    virtual ~Transform();

    const Vec3 &getLocalTranslation() const;
    void setLocalTranslation(const Vec3 &value);

    const Vec3 &getLocalScaling() const;
    void setLocalScaling(const Vec3 &value);

    const Quat &getLocalRotation() const;
    void setLocalRotation(const Quat &value);

    const Mat4& getLocalMatrix() const;
    const Mat4& getWorldMatrix() const;

    void setWorldTranslation(const Vec3 &t);
    void setWorldRotation(const Quat &r);

    TransformPtr getParentTransform() const;

    void refresh();

protected:
    void copyProperties(const Transform& inst);

protected:
    /**
     * @brief calcLocalTransformFromWorld
     * 直接使用世界状态反推本地状态
     * 忽略了scale，因为scale和rotate信息混合在了一起，无法分离。
     * 在大多数引擎，如doom3中，模型是没有scale信息的
     */
    void calcLocalTransformFromWorld();

    virtual void updateLocalMatrix();
    void updateWorldMatrix();

    void markLocalTransformRefreshFlag();
    void markWorldTransformRefreshFlag();

public:
    Signal<void()> transformRefresh;

protected:
    Vec3 localTranslation;
    Vec3 localScaling;
    Quat localRotation;

    Mat4 localMatrix;
    Mat4 worldMatrix;

    int refreshFlags;

protected:
    static const int RF_LOCAL_TRANSFORM = 0x01 << 0;
    static const int RF_WORLD_TRANSFORM = 0x01 << 1;
};

} // namespace re

#endif // RE_TRANSFORM_H
