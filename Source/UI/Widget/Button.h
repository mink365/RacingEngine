#ifndef BUTTON_H
#define BUTTON_H

#include "UI/Widget.h"
#include "UI/Base/Sprite.h"
#include "UI/Base/Label.h"

namespace re {

class BaseButton : public Widget
{
public:
    BaseButton();

    virtual void init();

    virtual void switchState(WidgetState newState);

    virtual void initTouchListener();

    void setOnClickFunc(std::function<void(ButtonPtr &)> func);
protected:
    virtual ComponentPtr createCloneInstance() const;
    virtual void copyProperties(const Component* component) override;

protected:
    std::function<void(ButtonPtr& button)> onButtonClickFunc;

    bool isTouchDown;
    int touchDownTime;
};

class ImageButton : public BaseButton
{
public:
    virtual void init(const string& texDefault, const string& texPress);
    virtual void init(const string& texDefault, const string& texPress, const string& texDis);

    virtual void switchState(WidgetState newState);

protected:
    virtual ComponentPtr createCloneInstance() const;
    virtual void copyProperties(const Component* component) override;

protected:
    SpritePtr defaultSprite, pressedSprite, disabledSprite;
};

class LabelButton : public ImageButton
{

protected:
    virtual ComponentPtr createCloneInstance() const;
    virtual void copyProperties(const Component* component) override;

protected:
    LabelPtr label;
};

}

#endif // BUTTON_H
