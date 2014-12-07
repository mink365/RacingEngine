/*
 * Color.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef COLOR_H_
#define COLOR_H_

#include "platform.h"

namespace re {

class Color {
public:
    static const Color White;
    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Cyan;
    static const Color Magenta;

public:
    float r, g, b, a;

    Color();
    Color(float r, float g, float b, float a);

    void set(float r, float g, float b, float a);

    Color operator *(const Color& v) const;

    Color& operator *=(const Color& v);

    std::string toString();
};

inline std::string Color::toString() {
    return StringUtil::Printf("R=%i,G=%i,B=%i,A=%i", (int)r*256, (int)g*256, (int)b*256, (int)a*256);
}

} // namespace re

#endif /* COLOR_H_ */
