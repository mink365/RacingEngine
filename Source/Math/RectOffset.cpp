#include "RectOffset.h"

namespace re {

RectOffset::RectOffset()
    : _left(0), _right(0), _bottom(0), _top(0)
{

}

RectOffset::RectOffset(float left, float right, float bottom, float top)
{
    _left = left;
    _right = right;
    _bottom = bottom;
    _top = top;
}

RectOffset::~RectOffset()
{

}

float RectOffset::getLeft() const
{
    return _left;
}

float RectOffset::getRight() const
{
    return _right;
}

float RectOffset::getBottom() const
{
    return _bottom;
}

float RectOffset::getTop() const
{
    return _top;
}

float RectOffset::getHorizontal() const
{
    return _left + _right;
}

float RectOffset::getVertical() const
{
    return _bottom + _top;
}

void RectOffset::setLeft(float v)
{
    _left = v;
}

void RectOffset::setRight(float v)
{
    _right = v;
}

void RectOffset::setBottom(float v)
{
    _bottom = v;
}

void RectOffset::setTop(float v)
{
    _top = v;
}

} // namespace re

