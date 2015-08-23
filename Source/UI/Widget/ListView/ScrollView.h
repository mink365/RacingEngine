#ifndef SCROLLVIEW_H
#define SCROLLVIEW_H

#include "UI/Widget.h"
#include "AxisData.h"
#include "VelocityTracker.h"

namespace re {
namespace ui {

class ScrollParam
{
public:
    ScrollParam() = default;
    ScrollParam(FlickableDirection dir, float flickDcc, int gridCount);

public:
    FlickableDirection scrollType = FlickableDirection::Horizonal;
    BoundBehavior boundBehavior = BoundBehavior::DragAndOvershootBound;

    float miniFlickVeclocity = 75;
    float flickThreshhold = 15;
    float flickDecc = 600;

    int gridCount = 0;
};

class ScrollView : public Widget
{
public:
    ScrollView();

    bool isHFlickable();
    bool isVFlickable();

    Vec2 GetMinExtent();
    Vec2 GetMaxExtent();

    void initTouchListener();

public:
    void setContainer(Transform2DPtr node);

public:
    void registerEvents();

protected:
    void onAwake();
    void onUpdate();

    void resetAxis();

private:
    void handleTouchDownEvent(TouchEvent& event);
    void handleTouchMoveEvent(TouchEvent& event);
    void handleTouchUpEvent(TouchEvent& event);

private:
    ScrollParam param;

    Vec2 pressTouch;
    Vec2 prevTouch;
    Vec2 currTouch;
    Vec2 diffTouch;

    AxisData hAxis;
    AxisData vAxis;

    Transform2DPtr container;

    VelocityTracker tracker;
    bool isMoved = false;
};

} // namespace ui
} // namespace re

#endif // SCROLLVIEW_H
