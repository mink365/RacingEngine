#ifndef AXISDATA_H
#define AXISDATA_H

#include "Math/Math.h"
#include "Message/Signal.h"

#include "ScrollAnimation.h"

namespace re {
namespace ui {

class ScrollParam;

enum class FlickableDirection
{
    Horizonal,
    Vertical,
    All,
    Auto,
};

enum class BoundBehavior
{
    StopAtGrid,
    StopAtBound,
    DragOverBound,
    DragAndOvershootBound,
};

enum class MoveState
{
    Stopped,
    Dragging,
    Flicking,
    Backing,
};

class AxisData
{
public:
    friend class ScrollView;

public:
    AxisData(const ScrollParam &param);

    void update();
    void reset();
    void drag(float diffValue);
    void flick(float velocity);
    void fixup();

protected:
    void switchState(MoveState state);

private:
    float minBound, maxBound;
    float axisLength, visibleLength;
    float startMargin, endMargin;

    float pressPos;
    float currentVelocity;
    float currentPos;
    float flickTarget;

    const ScrollParam& param;

    BackAnimation backAnim;
    FlipAnimation flipAnim;

    MoveState state = MoveState::Stopped;
};

} // namespace ui
} // namespace re

#endif // AXISDATA_H
