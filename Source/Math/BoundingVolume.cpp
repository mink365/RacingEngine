#include "BoundingVolume.h"

#include <algorithm>

re::Rect::Rect()
{

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
