#ifndef RE_TOUCHEVENT_H
#define RE_TOUCHEVENT_H

#include "Math/Vector.h"
#include "Base/Shared.h"

#include <functional>
#include <vector>
#include <memory>

namespace re {
namespace ui {

enum class TouchEventType {
    DOWN,
    MOVE,
    UP,
    CANCEL,
};

enum class WidgetTouchState {
    TOUCH_DOWN_INSIDE,
    TOUCH_DOWN_OUTSIDE,
    MOVE_INSIDE,
    MOVE_OUTSIDE,
    TOUCHUP_INSIDE,
    TOUCHUP_OUTSIDE,
    TOUCH_CANCLE,
};

class TouchEvent
{
public:
    TouchEvent();

    void setInfo(TouchEventType type, float x, float y);

    const Vec2& getCurrPoint() const;
    void setCurrPoint(const Vec2& v);

    const Vec2& getPoint() const;
    TouchEventType getType() const;

protected:
    TouchEventType type;

    Vec2 curr;
    Vec2 point;
};

class TouchEventListener : public Shared<TouchEventListener>
{
public:
    TouchEventListener();

public:
    std::function<bool(TouchEvent&, WidgetPtr&)> onTouchDown;
    std::function<void(TouchEvent&, WidgetPtr&)> onTouchDownOutSide;
    std::function<void(TouchEvent&, WidgetPtr&)> onTouchMoveInside;
    std::function<void(TouchEvent&, WidgetPtr&)> onTouchMoveOutside;
    std::function<void(TouchEvent&, WidgetPtr&)> onTouchMoveEnter;
    std::function<void(TouchEvent&, WidgetPtr&)> onTouchMoveOut;
    std::function<void(TouchEvent&, WidgetPtr&)> onTouchUpInside;
    std::function<void(TouchEvent&, WidgetPtr&)> onTouchUpOutside;
    std::function<void(TouchEvent&, WidgetPtr&)> onTouchCancle;
    std::function<void(WidgetTouchState oldTouchState, WidgetTouchState newTouchState,
                       TouchEvent& event, WidgetPtr& widget)> onTouchStateChange;
};

} // namespace ui
} // namespace re

#endif // RE_TOUCHEVENT_H
