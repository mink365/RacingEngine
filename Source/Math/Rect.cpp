/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "Rect.h"

#include "Matrix.h"

namespace re {

Rect::Rect()
{

}

Rect::Rect(const Vec2 &p, const Size &size)
{
    this->origin = p;
    this->size = size;
}

Rect::Rect(float x, float y, float width, float height)
{
    this->origin.x = x;
    this->origin.y = y;

    this->size.width = width;
    this->size.height = height;
}

bool Rect::compare(const Rect &v) const
{
    return (origin.compare(v.origin) && size.compare(v.size));
}

bool Rect::operator ==(const Rect &v) const
{
    return compare(v);
}

bool Rect::operator !=(const Rect &v) const
{
    return !compare(v);
}

void Rect::set(float x, float y, float width, float height)
{
    this->origin.x = x;
    this->origin.y = y;

    this->size.width = width;
    this->size.height = height;
}

void Rect::fromeVertex(const Vec2 &lb, const Vec2 &rt)
{
    this->origin = lb;
    this->size.width = rt.x - lb.x;
    this->size.height = rt.y - lb.y;
}

float Rect::getMinX() const
{
    return origin.x;
}

float Rect::getMidX() const
{
    return origin.x + size.width / 2.0f;
}

float Rect::getMaxX() const
{
    return origin.x + size.width;
}

float Rect::getMinY() const
{
    return origin.y;
}

float Rect::getMidY() const
{
    return origin.y + size.height / 2.0f;
}

float Rect::getMaxY() const
{
    return origin.y + size.height;
}

float Rect::getWidth() const
{
    return this->size.width;
}

float Rect::getHeight() const
{
    return this->size.height;
}

void Rect::setLeft(float v)
{
    float diff = this->origin.x - v;
    this->origin.x = v;

    this->size.width += diff;
}

void Rect::setRight(float v)
{
    this->size.width = v - this->origin.x;
}

void Rect::setBottom(float v)
{
    float diff = this->origin.y - v;
    this->origin.y = v;

    this->size.height += diff;
}

void Rect::setTop(float v)
{
    this->size.height = v - this->origin.y;
}

bool Rect::equals(const Rect &rect) const
{
    return compare(rect);
}

bool Rect::containsPoint(const Vec2 &point) const
{
    if (point.x < this->getMinX() || point.x > this->getMaxX()) {
        return false;
    }

    if (point.y < this->getMinY() || point.y > this->getMaxY()) {
        return false;
    }

    return true;
}

bool Rect::intersectsRect(const Rect &rect) const
{
    return !(     getMaxX() < rect.getMinX() ||
             rect.getMaxX() <      getMinX() ||
                  getMaxY() < rect.getMinY() ||
                  rect.getMaxY() <      getMinY());
}

bool Rect::containsRect(const Rect &rect) const
{
    return (this->containsPoint(rect.origin)
            && this->containsPoint(Vec2(rect.getMaxX(), rect.getMaxY())));
}

Rect Rect::unionWithRect(const Rect &rect) const
{
    float thisLeftX = origin.x;
    float thisRightX = origin.x + size.width;
    float thisTopY = origin.y + size.height;
    float thisBottomY = origin.y;

    if (thisRightX < thisLeftX)
    {
        std::swap(thisRightX, thisLeftX);   // This rect has negative width
    }

    if (thisTopY < thisBottomY)
    {
        std::swap(thisTopY, thisBottomY);   // This rect has negative height
    }

    float otherLeftX = rect.origin.x;
    float otherRightX = rect.origin.x + rect.size.width;
    float otherTopY = rect.origin.y + rect.size.height;
    float otherBottomY = rect.origin.y;

    if (otherRightX < otherLeftX)
    {
        std::swap(otherRightX, otherLeftX);   // Other rect has negative width
    }

    if (otherTopY < otherBottomY)
    {
        std::swap(otherTopY, otherBottomY);   // Other rect has negative height
    }

    float combinedLeftX = std::min(thisLeftX, otherLeftX);
    float combinedRightX = std::max(thisRightX, otherRightX);
    float combinedTopY = std::max(thisTopY, otherTopY);
    float combinedBottomY = std::min(thisBottomY, otherBottomY);

    return Rect(combinedLeftX, combinedBottomY, combinedRightX - combinedLeftX, combinedTopY - combinedBottomY);
}

string Rect::toString() const
{
    return StringUtil::Printf("Rect(%f, %f, %f, %f)", origin.x, origin.y, size.width, size.height);
}


Rect RectApplyMatrix(const Rect& rect, const Mat4& transform)
{
    float top    = rect.getMinY();
    float left   = rect.getMinX();
    float right  = rect.getMaxX();
    float bottom = rect.getMaxY();

    Vec3 topLeft(left, top, 0);
    Vec3 topRight(right, top, 0);
    Vec3 bottomLeft(left, bottom, 0);
    Vec3 bottomRight(right, bottom, 0);
    topLeft = transform * topLeft;
    topRight = transform * topRight;
    bottomLeft = transform * bottomLeft;
    bottomRight = transform * bottomRight;

    float minX = std::min(std::min(topLeft.x, topRight.x), std::min(bottomLeft.x, bottomRight.x));
    float maxX = std::max(std::max(topLeft.x, topRight.x), std::max(bottomLeft.x, bottomRight.x));
    float minY = std::min(std::min(topLeft.y, topRight.y), std::min(bottomLeft.y, bottomRight.y));
    float maxY = std::max(std::max(topLeft.y, topRight.y), std::max(bottomLeft.y, bottomRight.y));

    return Rect(minX, minY, (maxX - minX), (maxY - minY));
}


} // namespace re
