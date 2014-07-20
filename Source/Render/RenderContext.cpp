/*
 * RenderContext.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "RenderContext.h"

namespace re {

RenderContext::RenderContext() {
    this->boundIBO = 0;
    this->boundVBO = 0;

    this->textureUnits.resize(8);
}

RenderContext::~RenderContext() {
	// TODO Auto-generated destructor stub
}

}
