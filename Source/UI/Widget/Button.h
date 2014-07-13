#ifndef BUTTON_H
#define BUTTON_H

#include "UI/Widget.h"
#include "UI/Base/Sprite.h"
#include "UI/Base/Label.h"

namespace re {

class ClickListener : public Shared<ClickListener>
{
public:
    std::function<void(WidgetPtr& button)> onButtonClick;
};

class BaseButton : public Widget
{
public:
    BaseButton();

    virtual bool init();

    virtual void switchState(WidgetState newState);

    virtual void initTouchListener();

    virtual bool onTouchEvent(TouchEvent& event);

protected:
    virtual NodePtr createCloneInstance() const;
    virtual void copyProperties(const Node* node) override;

protected:
    ClickListener::ptr _clickListener;

    bool isTouchDown;
    int touchDownTime;
};

class ImageButton : public BaseButton
{
public:
    virtual void initView(const string& texDefault, const string& texPress, const string& texDis);

    virtual void switchState(WidgetState newState);

protected:
    virtual NodePtr createCloneInstance() const;
    virtual void copyProperties(const Node* node) override;

protected:
    SpritePtr defaultSprite, pressedSprite, disabledSprite;
};

class LabelButton : public ImageButton
{

protected:
    virtual NodePtr createCloneInstance() const;
    virtual void copyProperties(const Node* node) override;

protected:
    LabelPtr label;
};

}

#endif // BUTTON_H
