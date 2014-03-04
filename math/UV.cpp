/*
 * UV.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "UV.h"

reUv::reUv()
{
    this->u = 0;
    this->v = 0;
}

reUv::reUv(float u, float v)
{
    this->u = u;
    this->v = v;
}

void reUv::set(float u, float v)
{
    this->u = u;
    this->v = v;
}



