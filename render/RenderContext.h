/*
 * RenderContext.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef RENDERCONTEXT_H_
#define RENDERCONTEXT_H_

namespace re {

class RenderContext {
    friend class Renderer;
public:
	RenderContext();
	virtual ~RenderContext();

private:
    bool depthTestEnabled;
    int boundVBO;
    int boundIBO;
    int textureID;
};

} // namespace re

#endif /* RENDERCONTEXT_H_ */
