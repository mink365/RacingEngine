/*
 * Color.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "Color.h"

namespace re {

const Color Color::White(1, 1, 1, 1);
const Color Color::Black(0, 0, 0, 1);
const Color Color::Red(1, 0, 0, 1);
const Color Color::Green(0, 1, 0, 1);
const Color Color::Blue(0, 0, 1, 0);
const Color Color::Yellow(1, 1, 0, 1);
const Color Color::Cyan(0, 1, 1, 1);
const Color Color::Magenta(1, 0, 1, 1);

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


