/*
 * Renderer.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "Math/Matrix.h"
#include "Math/BoundingVolume.h"
#include "Texture/Texture.h"
#include "RenderState.h"
#include "Geometry/Geometry.h"
#include "RenderContext.h"
#include "Scene/Camera/Camera.h"

namespace re {

class Shader;
class RenderTarget;

class RenderView : public Shared<RenderView>
{
public:
    void init(CameraPtr& camera);
    void init(LightPtr& light);

public:
    CameraProjectionMode mode;

    Rect viewport;

    Mat4 viewMatrix;
    Mat4 projMatrix;

    float zNear;
    float zFar;

    union {
        float fov;
        float orthoWidth;
    };

    int clearFlag;
    std::function<bool(int queueID)> queueCullFunc;

    std::shared_ptr<Shader> forceShader;
    std::shared_ptr<RenderTarget> renderTarget;
};

class Renderer {
public:
	Renderer();
	virtual ~Renderer();

    virtual void setViewport(const Rect& viewport) = 0;
    void setWorldMatrix(const Mat4 &mat);
    void setViewMatrix(const Mat4 &mat);
    void setProjectionMatrix(const Mat4 &mat);

    virtual void setTexture(int unit, bool enable, const Texture &texture) = 0;

    virtual void bindRenderTarget(const RenderTarget &target) = 0;
    virtual void resetRenderTarget() = 0;
    virtual void setupRenderTarget(RenderTarget &target) = 0;

    virtual void bindShader(const Shader &shader) = 0;
    virtual void bindBuffer(const Geometry &geometry) = 0;

    virtual void renderMesh(const Geometry &geometry) = 0;

    virtual void cleanBuffers(bool color, bool depth, bool stencil) = 0;
    virtual void cleanBuffers(int flag) = 0;

    virtual void applyRenderState(const RenderState &state, bool force=false) = 0;

    /**
     * @brief resetToRenderState
     * @param state
     * 忽略本地的状态缓冲，直接置为指定状态
     */
    void resetToRenderState(const RenderState &state);

protected:
    virtual void activateTextureUnit(int unit) = 0;

protected:
    RenderContext context;

    Mat4 viewMatrix, projMatrix;
};

} // namespace re

#endif /* RENDERER_H_ */
