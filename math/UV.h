/*
 * UV.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef UV_H_
#define UV_H_

class reUv {
public:
    float u;
    float v;

    reUv();
    reUv(float u, float v);

    void set(float u, float v);
};

#endif /* UV_H_ */
