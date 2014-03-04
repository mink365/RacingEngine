/*
 * Renderer.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <math/Matrix.h>
#include <texture/Texture.h>
#include "RenderState.h"
#include <geometry/Geometry.h>
#include "RenderContext.h"

class Renderer {
public:
	Renderer();
	virtual ~Renderer();

    void setViewPort(int x, int y, int width, int height);
    void setWorldMatrix(const reMat4 &mat);
    void setViewMatrix(const reMat4 &mat);
    void setProjectionMatrix(const reMat4 &mat);

    void setTexture(int unit, bool enable, const Texture &texture);
    void setTextureMatrix(int unit, const reMat4 &mat);

    void bindBuffer(const Geometry &geometry);
    void renderMesh(const Geometry &geometry);

    void cleanBuffers(bool color, bool depth, bool stencil);

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

#endif /* RENDERER_H_ */
