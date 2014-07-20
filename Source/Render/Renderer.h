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

namespace re {

class Renderer {
public:
	Renderer();
	virtual ~Renderer();

    void setViewPort(int x, int y, int width, int height);
    void setWorldMatrix(const Mat4 &mat);
    void setViewMatrix(const Mat4 &mat);
    void setProjectionMatrix(const Mat4 &mat);

    void setTexture(int unit, bool enable, const Texture &texture);

    void bindBuffer(const Geometry &geometry);
    void renderMesh(const Geometry &geometry);

    void cleanBuffers(bool color, bool depth, bool stencil);
    void cleanBuffers(int flag);

    void applyRenderState(const RenderState &state, bool force=false);

    /**
     * @brief resetToRenderState
     * @param state
     * 忽略本地的状态缓冲，直接置为指定状态
     */
    void resetToRenderState(const RenderState &state);

private:
    void activateTextureUnit(int unit);

private:
    RenderContext context;

    Rect viewport;
    Mat4 viewMatrix, projMatrix;
};

} // namespace re

#endif /* RENDERER_H_ */
