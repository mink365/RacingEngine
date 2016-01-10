/*
 * RenderContext.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef RENDERCONTEXT_H_
#define RENDERCONTEXT_H_

#include "platform.h"
#include "RenderState.h"
#include "Math/Rect.h"
#include <vector>

namespace re {

class RenderTarget;
using RenderTargetPtr = SharedPtr<RenderTarget>;

class RenderContext {
    friend class Renderer;
    friend class GLES2Renderer;

public:
	RenderContext();
	virtual ~RenderContext();

private:
    GPU_ID boundVBO;
    GPU_ID boundIBO;
    GPU_ID boundFBO;

    Rect viewport;

    RenderState renderState;

    std::vector<TextureUnitContextState> textureUnits;

    // RenderTarget of framebuffer
    RenderTargetPtr defaultRenderTarget;
};

} // namespace re

#endif /* RENDERCONTEXT_H_ */
