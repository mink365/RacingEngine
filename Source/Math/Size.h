/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_SIZE_H
#define RE_SIZE_H

namespace re {

class Size
{
public:
    Size();
    Size(float w, float h);

    Size& set(const Size& v);
    Size& set(float w, float h);

    bool compare(const Size& v) const;
    bool operator ==(const Size& v) const;
    bool operator !=(const Size& v) const;

public:
    float width;
    float height;
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

} // namespace re

#endif // RE_SIZE_H
