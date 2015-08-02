#ifndef AXISDATA_H
#define AXISDATA_H

namespace re
{

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
    AxisData();

    void update();
    void reset();
    void drag(float diffValue);
    void flick(float velocity);
    void fixup();

protected:
    void changState(MoveState state);

private:
    float minBound, maxBound;
    float axisLength, visibleLength;
    float startMargin, endMargin;

    float pressPos;
    float currentVelocity;
    float currentPos;
    float flickTarget;

    MoveState state;
};

}

#endif // AXISDATA_H
