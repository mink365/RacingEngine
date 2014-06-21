#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include "Vector.h"

namespace re {

class Size
{
public:
    float width;
    float height;

public:
    Size();
    Size(float w, float h);

    Size& set(const Size& v);
    Size& set(float w, float h);

    bool compare(const Size& v) const;
    bool operator ==(const Size& v) const;
    bool operator !=(const Size& v) const;
};

inline Size::Size()
    :width(0), height(0)
{

}

inline Size::Size(float w, float h)
{
    this->width = w;
    this->height = h;
}

inline Size &Size::set(const Size &v)
{
    *this = v;

    return *this;
}

inline Size &Size::set(float w, float h)
{
    this->width = w;
    this->height = h;

    return *this;
}

inline bool Size::compare(const Size &v) const
{
    return ((width  == v.width) && (height == v.height));
}

inline bool Size::operator ==(const Size &v) const
{
    return compare(v);
}

inline bool Size::operator !=(const Size &v) const
{
    return !compare(v);
}

class Rect
{
public:
    Vec2 origin;
    Size size;

public:
    Rect();
    Rect(float x, float y, float width, float height);

    bool compare(const Rect& v) const;
    bool operator ==(const Rect& v) const;
    bool operator !=(const Rect& v) const;

    float getMinX() const;
    float getMidX() const;
    float getMaxX() const;

    float getMinY() const;
    float getMidY() const;
    float getMaxY() const;

    bool equals(const Rect& rect) const;

    bool containsPoint(const Vec2& point) const;
    bool intersectsRect(const Rect& rect) const;
    Rect unionWithRect(const Rect & rect) const;
};

}

#endif // BOUNDINGVOLUME_H
