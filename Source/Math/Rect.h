/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_RECT_H
#define RE_RECT_H

#include "Size.h"
#include "Vector.h"

namespace re {

class Mat4;

class Rect
{
public:
    Vec2 origin;
    Size size;

public:
    Rect();
    Rect(const Vec2& p, const Size& size);
    Rect(float x, float y, float width, float height);

    bool compare(const Rect& v) const;
    bool operator ==(const Rect& v) const;
    bool operator !=(const Rect& v) const;

    void set(float x, float y, float width, float height);
    void fromeVertex(const Vec2& lb, const Vec2& rt);

    float getMinX() const;
    float getMidX() const;
    float getMaxX() const;

    float getMinY() const;
    float getMidY() const;
    float getMaxY() const;

    float getWidth() const;
    float getHeight() const;

    void setLeft(float v);
    void setRight(float v);
    void setBottom(float v);
    void setTop(float v);

    bool equals(const Rect& rect) const;

    bool containsPoint(const Vec2& point) const;
    bool intersectsRect(const Rect& rect) const;
    bool containsRect(const Rect& rect) const;
    Rect unionWithRect(const Rect & rect) const;
};

Rect RectApplyMatrix(const Rect &rect, const Mat4 &m);

} // namespace re

#endif // RE_RECT_H
