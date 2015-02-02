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
    static const Color Gray;
    static const Color DarkGray;
    static const Color LightGray;
    static const Color Orang;
    static const Color Brown;
    static const Color Pink;
    static const Color BlackNoAlpha;

public:
    Color();
    Color(float r, float g, float b, float a);

    void set(float r, float g, float b, float a);

    Color& operator +=(const Color& v);
    Color& operator -=(const Color& v);
    Color& operator *=(float rhs);
    Color& operator /=(float rhs);
    Color& operator *=(const Color& rhs);

    Color operator +(const Color& v) const;
    Color operator -(const Color& v) const;
    Color operator *(float rhs) const;
    Color operator /(float rhs) const;
    Color operator *(const Color& v) const;

    const Color operator +() const;
    const Color operator -() const;

public:
    static Color FromHex(const std::string& hex);

    std::string toString();

public:
    float r, g, b, a;
};

inline std::string Color::toString()
{
    return StringUtil::Printf("Color(%i, %i, %i, %i)", (int)r*256, (int)g*256, (int)b*256, (int)a*256);
}

} // namespace re

#endif /* COLOR_H_ */
