/*
 * RenderContext.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef RENDERCONTEXT_H_
#define RENDERCONTEXT_H_

#include "RenderState.h"
#include <vector>

namespace re {

class RenderContext {
    friend class Renderer;
public:
	RenderContext();
	virtual ~RenderContext();

private:
    int boundVBO;
    int boundIBO;

    BlendState blendState;
    AlphaState alphaState;
    StencilState stencilState;
    DepthState depthState;
    FaceCullState faceCullState;

    std::vector<TextureUnitContexState> textureUnits;
};

} // namespace re

#endif /* RENDERCONTEXT_H_ */
