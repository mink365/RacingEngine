/*
 * Renderer.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "Math/Matrix.h"
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
    void setTextureMatrix(int unit, const Mat4 &mat);

    void bindBuffer(const Geometry &geometry);
    void renderMesh(const Geometry &geometry);

    void cleanBuffers(bool color, bool depth, bool stencil);
    void cleanBuffers(int flag);

    void applyRenderState(const RenderState &state);

    /**
     * @brief resetToRenderState
     * @param state
     * 忽略本地的状态缓冲，直接置为指定状态
     */
    void resetToRenderState(const RenderState &state);
private:
    RenderContext context;
};

} // namespace re

#endif /* RENDERER_H_ */
