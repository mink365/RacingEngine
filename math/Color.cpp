/*
 * Color.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Color.h"

namespace re {

Color::Color()
{
    this->r = 1;
    this->g = 1;
    this->b = 1;
    this->a = 1;
}

Color::Color(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void Color::set(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

}


