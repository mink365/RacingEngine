#ifndef SCROLLVIEW_H
#define SCROLLVIEW_H

#include "UI/Widget.h"
#include "AxisData.h"
#include "VelocityTracker.h"

namespace re {

class ScrollParam
{
public:
    ScrollParam(FlickableDirection dir, float flickDcc, int gridCount);

public:
    FlickableDirection scrollType = FlickableDirection::Horizonal;
    BoundBehavior boundBehavior = BoundBehavior::DragAndOvershootBound;

    float miniFlickVeclocity = 75;
    float flickThreshhold = 15;
    float flickDecc = 600;

    int gridCount;
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

protected:
    void update();

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

    Transform2DPtr transform;
    Transform2DPtr container;

    VelocityTracker tracker;
    bool isMoved;
};

}

#endif // SCROLLVIEW_H
