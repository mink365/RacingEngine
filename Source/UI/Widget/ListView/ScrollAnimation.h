#ifndef RE_UI_SCROLLANIMATION_H
#define RE_UI_SCROLLANIMATION_H

#include "Message/Signal.h"

namespace re {
namespace ui {

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
    float _runTime = 0;
    float _duration = 0;

    State _state = State::Stopped;

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
    float _distance = 0;
    float _endPos = 0;
};

class FlipAnimation : public BaseAnimation
{
public:
    void start(float currPos, float velocity);
    float getPosition();

    float calculateStartVelocity(float currPos, float endPos);

private:
    float _endPos = 0;
    float _realDcc = 0;
public:
    float _dcc = 0;
};

} // namespace ui
} // namespace re

#endif // RE_UI_SCROLLANIMATION_H
