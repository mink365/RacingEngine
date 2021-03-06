#include "TouchEvent.h"

namespace re {
namespace ui {

TouchEvent::TouchEvent()
{
}

void TouchEvent::setInfo(TouchEventType type, float x, float y)
{
    this->type = type;
    this->point.set(x, y);
}

/**
 * @brief TouchEvent::getCurrPoint
 * point in the local space of widget
 * @return
 */
const Vec2 &TouchEvent::getCurrPoint() const
{
    return this->curr;
}

void TouchEvent::setCurrPoint(const Vec2 &v)
{
    this->curr = v;
}

/**
 * @brief TouchEvent::getPoint
 * point in the screen space
 * @return
 */
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
    this->onTouchStateChange = [](WidgetTouchState, WidgetTouchState,
            TouchEvent&, WidgetPtr){};
}

} // namespace ui
} // namespace re
