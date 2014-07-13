#include "TouchEvent.h"

namespace re {

TouchEvent::TouchEvent()
{
}

void TouchEvent::setInfo(TouchEventType type, float x, float y)
{
    this->type = type;
    this->point.set(x, y);
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

TouchEventListener::TouchEventListener()
{
    this->onTouchDown = [](TouchEvent&, WidgetPtr){return false;};
    this->onTouchDownOutSide = [](TouchEvent&, WidgetPtr){};
    this->onTouchMoveEnter = [](TouchEvent&, WidgetPtr){};
    this->onTouchMoveInside = [](TouchEvent&, WidgetPtr){};
    this->onTouchMoveOutside = [](TouchEvent&, WidgetPtr){};
    this->onTouchMoveOut = [](TouchEvent&, WidgetPtr){};
    this->onTouchUpInside = [](TouchEvent&, WidgetPtr){};
    this->onTouchUpOutside = [](TouchEvent&, WidgetPtr){};
    this->onTouchCancle = [](TouchEvent&, WidgetPtr){};
    this->onTouchStateChange = [](WidgetTouchState oldTouchState, WidgetTouchState newTouchState,
            TouchEvent& event, WidgetPtr widget){};
}

} // namespace re
