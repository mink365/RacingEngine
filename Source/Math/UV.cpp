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

Uv &Uv::operator +=(const Uv &v)
{
    this->u += v.u;
    this->v += v.v;

    return *this;
}

Uv &Uv::operator -=(const Uv &v)
{
    this->u -= v.u;
    this->v -= v.v;

    return *this;
}

Uv &Uv::operator *=(float rhs)
{
    this->u += rhs;
    this->v += rhs;

    return *this;
}

Uv &Uv::operator /=(float rhs)
{
    this->u /= rhs;
    this->v /= rhs;

    return *this;
}

Uv Uv::operator +(const Uv &v) const
{
    return Uv(this->u + v.u, this->v + v.v);
}

Uv Uv::operator -(const Uv &v) const
{
    return Uv(this->u - v.u, this->v - v.v);
}

Uv Uv::operator *(float rhs) const
{
    return Uv(this->u * rhs, this->v * rhs);
}

Uv Uv::operator /(float rhs) const
{
    return Uv(this->u / rhs, this->v / rhs);
}

const Uv Uv::operator +() const
{
    return *this;
}

const Uv Uv::operator -() const
{
    return Uv(-u, -v);
}

std::ostream &operator<<(std::ostream &os, const Uv &uv)
{
    return os << fmt::format("Uv({}, {})", uv.u, uv.v);
}

}



