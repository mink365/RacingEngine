#ifndef AXISDATA_H
#define AXISDATA_H

#include "Math/Math.h"
#include "Message/Signal.h"

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

class BaseAnimation
{
public:
    enum class State
    {
        Running,
        Stopped,
    };

public:
    virtual void update();
    void start();
    void stop();

private:
    void switchState(State state);

protected:
    float _runTime;
    float _duration;

    State _state;

public:
    Signal<void(State)> stateEvent;
};

class BackAnimation : public BaseAnimation
{
public:
    void start(float currPos, float endPos, float time);
    float getPosition();
    float getDistance();

private:
    float _distance;
    float _endPos;
};

class FlipAnimation : public BaseAnimation
{
public:
    void start(float currPos, float velocity);
    float getPosition();

    float calculateStartVelocity(float currPos, float endPos);

private:
    float _endPos;
    float _realDcc;
public:
    float _dcc;
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

    MoveState state;
};

} // namespace ui
} // namespace re

#endif // AXISDATA_H
