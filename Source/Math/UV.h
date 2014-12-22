/*
 * UV.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef UV_H_
#define UV_H_

#include "platform.h"

namespace re {

class Uv {
public:
    Uv();
    Uv(float u, float v);

    void set(float u, float v);

    Uv& operator +=(const Uv& v);
    Uv& operator -=(const Uv& v);
    Uv& operator *=(float rhs);
    Uv& operator /=(float rhs);

    Uv operator +(const Uv& v) const;
    Uv operator -(const Uv& v) const;
    Uv operator *(float rhs) const;
    Uv operator /(float rhs) const;

    const Uv operator +() const;
    const Uv operator -() const;

    std::string toString() const;

public:
    float u;
    float v;
};

inline std::string Uv::toString() const
{
    return StringUtil::Printf("Uv(%f, %f)", u, v);
}

} // namespace re

#endif /* UV_H_ */
