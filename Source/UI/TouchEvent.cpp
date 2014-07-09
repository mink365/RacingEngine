#include "TouchEvent.h"

namespace re {

TouchEvent::TouchEvent()
{
}

const Vec2 &TouchEvent::getCurrPoint() const
{
    return this->curr;
}

void TouchEvent::setCurrPoint(const Vec2 &v)
{
    this->curr = v;
}

const Vec2 &TouchEvent::getPoint() const
{
    return this->point;
}

TouchEventType TouchEvent::getType() const
{
    return this->type;
}

} // namespace re
