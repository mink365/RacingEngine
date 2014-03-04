#ifndef CAMERA_H
#define CAMERA_H

#include <scene/SceneNode.h>

class Camera : public SceneNode
{
public:
    Camera();

    void setDepthField(float near, float far);
    void setViewport(float width, float height);

    /**
     * @brief setView
     * @param center
     * @param eye
     * @param up 大概的，整个世界的up向量
     */
    void setView(const reVec3 &center, const reVec3 &eye, const reVec3 &up);

    /**
     * @brief setAxes
     * @param left
     * @param up
     * @param direction
     *
     * 使用三个轴确定node的旋转状态
     */
    void setAxes(const reVec3 &left, const reVec3 &up, const reVec3 &direction);

    const reMat4 &getViewMatrix() const;
    const reMat4 &getProjectionMatrix() const;
    const reMat4 &getViewProjectionMatrix() const;

protected:
    void recalcViewMatrix();
    void recalcProjectionMatrix();
    void onChange();

private:
    reVec3 center;
    reVec3 up;
    reVec3 left;

    reMat4 viewMatrix;
    reMat4 projectionMatrix;
    reMat4 viewProjectionMatrix;

    float zNear;
    float zFar;

    float viewportWidth;
    float viewportHeight;
};

#endif // CAMERA_H
