/*
 * Renderer.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "Math/Matrix.h"
#include "Math/Color.h"
#include "Math/Rect.h"
#include "RenderContext.h"

namespace re {

class Shader;
class Texture;
class RenderTarget;
class RenderState;
class MeshData;

class Renderer
{
public:
	Renderer();
	virtual ~Renderer();

    virtual void setViewport(const Rect& viewport) = 0;
    void setModelMatrix(const Mat4 &mat);
    void setViewMatrix(const Mat4 &mat);
    void setProjectionMatrix(const Mat4 &mat);
    const Mat4& getModelMatrix() const;
    const Mat4& getViewMatrix() const;
    const Mat4& getProjectionMatrix() const;

    void setDefaultRenderbuffer(uint32_t width, uint32_t height, uint32_t framebuffer,
                                uint32_t colorRenderbuffer, uint32_t depthRenderbuffer, bool depth, bool stencil);

    virtual void bindTexture(int unit, bool enable, const Texture &texture) = 0;

    virtual void bindRenderTarget(const RenderTarget &target) = 0;
    virtual void resetRenderTarget();

    virtual void setupRenderTarget(RenderTarget &target) = 0;

    virtual void bindShader(const Shader &shader) = 0;
    virtual void bindBuffer(const MeshData &mesh) = 0;

    virtual void renderMesh(const MeshData &mesh) = 0;

    virtual void setClearColor(const Color& color);
    virtual void cleanBuffers(bool color, bool depth, bool stencil) = 0;
    virtual void cleanBuffers(int flag);

    virtual void applyRenderState(const RenderState &state, bool force=false) = 0;

    /**
     * @brief resetToRenderState
     * @param state
     * force sync hardward state to RenderState set here
     */
    void resetToRenderState(const RenderState &state);

    void readStatistics(size_t* drawCall, size_t* trangles);
    void resetStatistics();

protected:
    virtual void activateTextureUnit(int unit) = 0;

    void addDrawCall();
    void addDrawTrangles(size_t count);

protected:
    RenderContext context;

    Mat4 modelMatrix, viewMatrix, projMatrix;

    Color _clearColor;

    size_t drawCall, trangles;
};

} // namespace re

#endif /* RENDERER_H_ */
