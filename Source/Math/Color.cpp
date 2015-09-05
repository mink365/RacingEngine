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
const Color Color::Gray(0.5, 0.5, 0.5, 1);
const Color Color::DarkGray(0.2, 0.2, 0.2, 1);
const Color Color::LightGray(0.8, 0.8, 0.8, 1);
const Color Color::Orang(251/255.0f, 130/255.0f, 0, 1);
const Color Color::Brown(65/255.0f, 40/255.0f, 25/255.0f, 1);
const Color Color::Pink(1, 0.68, 0.68, 0);
const Color Color::BlackNoAlpha(0, 0, 0, 0);

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

Color &Color::operator +=(const Color &v)
{
    this->r += v.r;
    this->g += v.g;
    this->b += v.b;
    this->a += v.a;

    return *this;
}

Color &Color::operator -=(const Color &v)
{
    this->r -= v.r;
    this->g -= v.g;
    this->b -= v.b;
    this->a -= v.a;

    return *this;
}

Color &Color::operator *=(float rhs)
{
    this->r *= rhs;
    this->g *= rhs;
    this->b *= rhs;
    this->a *= rhs;

    return *this;
}

Color &Color::operator /=(float rhs)
{
    this->r /= rhs;
    this->g /= rhs;
    this->b /= rhs;
    this->a /= rhs;

    return *this;
}

Color &Color::operator *=(const Color &rhs)
{
    this->r *= rhs.r;
    this->g *= rhs.g;
    this->b *= rhs.b;
    this->a *= rhs.a;

    return *this;
}

Color Color::operator +(const Color &v) const
{
    Color color = *this;

    color += v;

    return color;
}

Color Color::operator -(const Color &v) const
{
    Color color = *this;

    color -= v;

    return color;
}

Color Color::operator *(float rhs) const
{
    Color color = *this;

    color *= rhs;

    return color;
}

Color Color::operator /(float rhs) const
{
    Color color = *this;

    color /= rhs;

    return color;
}

Color Color::operator *(const Color &v) const
{
    Color color = *this;

    color *= v;

    return color;
}

const Color Color::operator +() const
{
    return *this;
}

const Color Color::operator -() const
{
    return Color(-r, -g, -b, -a);
}

Color Color::FromHex(const string &hex)
{
    int start = 0;
    int len = hex.size();

    if (hex[0] == '#') {
        start = 1;
        len -= 1;
    }

    Color color;
    if (len >= 6) {
        color.r = std::stoi(hex.substr(start, 2), 0, 16) / 255.0f;
        color.g = std::stoi(hex.substr(start + 2, 2), 0, 16) / 255.0f;
        color.b = std::stoi(hex.substr(start + 4, 2), 0, 16) / 255.0f;

        if (len == 8) {
            color.a = std::stoi(hex.substr(start + 6, 2), 0, 16) / 255.0f;
        }
    } else {
        color.r = std::stoi(hex.substr(start, 1), 0, 16) / 15.0f;
        color.g = std::stoi(hex.substr(start + 1, 1), 0, 16) / 15.0f;
        color.b = std::stoi(hex.substr(start + 2, 1), 0, 16) / 15.0f;

        if (len == 4) {
            color.a = std::stoi(hex.substr(start + 3, 1), 0, 16) / 15.0f;
        }
    }

    return color;
}

std::ostream &operator<<(std::ostream &os, const Color& color)
{
    os << fmt::format("Color({}, {}, {}, {})", (int)color.r*256, (int)color.g*256, (int)color.b*256, (int)color.a*256);
    return os;
}

}


