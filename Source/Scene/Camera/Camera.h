#ifndef CAMERA_H
#define CAMERA_H

#include <functional>
#include "Base/ECS/Component.h"
#include "Scene/Transform.h"
#include "Render/RenderTarget.h"
#include "Math/Rect.h"
#include "Math/Color.h"

namespace re {

class Camera;
using CameraPtr = ComponentHandle<Camera>;

enum class CameraProjectionMode {
    Perspective,
    Orthographic
};

class Camera : public Component<Camera>
{
    friend class RenderView;

public:
    Camera();

    void setDepthField(float near, float far);
    void setViewport(float width, float height);
    void setViewport(Rect v);
    const Rect& getViewport() const;

    void setProjectionMode(CameraProjectionMode mode);
    CameraProjectionMode getProjectionMode() const;

    void setFieldOfView(float fov);
    float getFieldOfView() const;

    void setOrthoWidth(float v);
    Size getOrthoSize() const;

    /**
     * @brief setView
     * @param center
     * @param eye
     * @param up 大概的，整个世界的up向量
     */
    void setView(const Vec3 &eye, const Vec3 &center, const Vec3 &up);

    /**
     * @brief setAxes
     * @param left
     * @param up
     * @param direction
     *
     * 使用三个轴确定node的旋转状态
     */
    void setAxes(const Vec3 &left, const Vec3 &up, const Vec3 &direction);

    const Mat4 &getViewMatrix() const;
    const Mat4 &getProjectionMatrix() const;
    const Mat4 &getViewProjectionMatrix() const;

    RenderTargetPtr getRenderTarget() const;
    void setRenderTarget(RenderTargetPtr target);

    void setBuffersClearFlag(int flag);
    int getBuffersClearFlag() const;
    void setClearColor(const Color& color);
    const Color& getClearColor() const;
    void setQueueCullFunc(std::function<bool(int queueID)> func);
    std::function<bool(int queueID)> getQueueCullFunc() const;

public:
    void registerEvents();

protected:
    void onEnter();

protected:
    void recalcViewMatrix();
    void recalcProjectionMatrix();
    void onChange();

private:
    TransformPtr transform;

private:
    Vec3 center;
    Vec3 up;
    Vec3 left;

    Mat4 viewMatrix;
    Mat4 projectionMatrix;
    Mat4 viewProjectionMatrix;

    CameraProjectionMode mode;

    float zNear;
    float zFar;

    float fov;
    float orthoWidth;

    Rect viewport;

    RenderTargetPtr renderTarget;

    int clearFlag;
    Color clearColor;
    std::function<bool(int queueID)> queueCullFunc;
};

} // namespace re

#endif // CAMERA_H
