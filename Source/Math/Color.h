/*
 * Color.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef COLOR_H_
#define COLOR_H_

namespace re {

class Color {
public:
    static const Color WHITE;
    static const Color BLACK;

public:
    float r, g, b, a;

    Color();
    Color(float r, float g, float b, float a);

    void set(float r, float g, float b, float a);

    Color operator *(const Color& v) const;

    Color& operator *=(const Color& v);
};

} // namespace re

#endif /* COLOR_H_ */
