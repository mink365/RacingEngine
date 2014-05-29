/*
 * UV.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "UV.h"

namespace re {

Uv::Uv()
{
    this->u = 0;
    this->v = 0;
}

Uv::Uv(float u, float v)
{
    this->u = u;
    this->v = v;
}

void Uv::set(float u, float v)
{
    this->u = u;
    this->v = v;
}

}



