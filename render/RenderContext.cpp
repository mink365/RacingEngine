/*
 * RenderContext.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "RenderContext.h"

RenderContext::RenderContext() {
    this->depthTestEnabled = false;
    this->boundIBO = 0;
    this->boundVBO = 0;
    this->textureID = 0;
}

RenderContext::~RenderContext() {
	// TODO Auto-generated destructor stub
}

