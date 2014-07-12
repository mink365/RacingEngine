/*
 * Color.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Color.h"

namespace re {

const Color Color::WHITE(1, 1, 1, 1);
const Color Color::BLACK(0, 0, 0, 1);

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

Color Color::operator *(const Color &v) const
{
    Color color = *this;

    color *= v;

    return color;
}

Color &Color::operator *=(const Color &v)
{
    this->r *= v.r;
    this->g *= v.g;
    this->b *= v.b;
    this->a *= v.a;

    return *this;
}

}


