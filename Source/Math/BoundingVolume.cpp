#include "BoundingVolume.h"

#include "Vector.h"
#include "Matrix.h"

#include <algorithm>
#include <math.h>

using namespace std;

re::Rect::Rect()
{

}

re::Rect::Rect(const re::Vec2 &p, const re::Size &size)
{
    this->origin = p;
    this->size = size;
}

re::Rect::Rect(float x, float y, float width, float height)
{
    this->origin.x = x;
    this->origin.y = y;

    this->size.width = width;
    this->size.height = height;
}

bool re::Rect::compare(const re::Rect &v) const
{
    return (origin.compare(v.origin) && size.compare(v.size));
}

bool re::Rect::operator ==(const re::Rect &v) const
{
    return compare(v);
}

bool re::Rect::operator !=(const re::Rect &v) const
{
    return !compare(v);
}

void re::Rect::set(float x, float y, float width, float height)
{
    this->origin.x = x;
    this->origin.y = y;

    this->size.width = width;
    this->size.height = height;
}

void re::Rect::fromeVertex(const re::Vec2 &lb, const re::Vec2 &rt)
{
    this->origin = lb;
    this->size.width = rt.x - lb.x;
    this->size.height = rt.y - lb.y;
}

float re::Rect::getMinX() const
{
    return origin.x;
}

float re::Rect::getMidX() const
{
    return origin.x + size.width / 2.0f;
}

float re::Rect::getMaxX() const
{
    return origin.x + size.width;
}

float re::Rect::getMinY() const
{
    return origin.y;
}

float re::Rect::getMidY() const
{
    return origin.y + size.height / 2.0f;
}

float re::Rect::getMaxY() const
{
    return origin.y + size.height;
}

float re::Rect::getWidth() const
{
    return this->size.width;
}

float re::Rect::getHeight() const
{
    return this->size.height;
}

void re::Rect::setLeft(float v)
{
    this->origin.x = v;
}

void re::Rect::setRight(float v)
{
    this->size.width = v - this->origin.x;
}

void re::Rect::setBottom(float v)
{
    this->origin.y = v;
}

void re::Rect::setTop(float v)
{
    this->size.height = v - this->origin.y;
}

bool re::Rect::equals(const re::Rect &rect) const
{
    return compare(rect);
}

bool re::Rect::containsPoint(const re::Vec2 &point) const
{
    if (point.x < this->getMinX() || point.x > this->getMaxX()) {
        return false;
    }

    if (point.y < this->getMinY() || point.y > this->getMaxY()) {
        return false;
    }

    return true;
}

bool re::Rect::intersectsRect(const re::Rect &rect) const
{
    return !(     getMaxX() < rect.getMinX() ||
             rect.getMaxX() <      getMinX() ||
                  getMaxY() < rect.getMinY() ||
                  rect.getMaxY() <      getMinY());
}

bool re::Rect::containsRect(const re::Rect &rect) const
{
    return (this->containsPoint(rect.origin)
            && this->containsPoint(Vec2(rect.getMaxX(), rect.getMaxY())));
}

re::Rect re::Rect::unionWithRect(const re::Rect &rect) const
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


re::Rect re::RectApplyMatrix(const re::Rect& rect, const re::Mat4& transform)
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

    float minX = min(min(topLeft.x, topRight.x), min(bottomLeft.x, bottomRight.x));
    float maxX = max(max(topLeft.x, topRight.x), max(bottomLeft.x, bottomRight.x));
    float minY = min(min(topLeft.y, topRight.y), min(bottomLeft.y, bottomRight.y));
    float maxY = max(max(topLeft.y, topRight.y), max(bottomLeft.y, bottomRight.y));

    return Rect(minX, minY, (maxX - minX), (maxY - minY));
}
