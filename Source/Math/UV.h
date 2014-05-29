/*
 * UV.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef UV_H_
#define UV_H_

namespace re {

class Uv {
public:
    float u;
    float v;

    Uv();
    Uv(float u, float v);

    void set(float u, float v);
};

} // namespace re

#endif /* UV_H_ */
