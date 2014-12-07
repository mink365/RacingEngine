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
    float u;
    float v;

    Uv();
    Uv(float u, float v);

    void set(float u, float v);

    std::string toString() const;
};

inline std::string Uv::toString() const
{
    return StringUtil::Printf("Uv(%f, %f)", u, v);
}

} // namespace re

#endif /* UV_H_ */
