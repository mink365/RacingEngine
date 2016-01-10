/*
 * RenderContext.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "RenderContext.h"
#include "RenderTarget.h"

namespace re {

RenderContext::RenderContext() {
    this->boundIBO = 0;
    this->boundVBO = 0;
    this->boundFBO = 0;

    this->textureUnits.resize(8);

    this->defaultRenderTarget = Create<RenderTarget>();
}

RenderContext::~RenderContext() {

}

}
