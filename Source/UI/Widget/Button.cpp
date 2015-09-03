#include "Button.h"

#include "Util/ComponentFactory.h"

namespace re {
namespace ui {

Button::Button()
{
}

void Button::onAwake()
{
    this->initTouchListener();

    this->onButtonClickFunc = nullptr;
}

void Button::switchState(WidgetState newState)
{
    if (this->state == newState) {
        return;
    }

    WidgetState oldState = this->state;
    this->state = newState;

    this->switchStateForImage(oldState, newState);
}

void Button::initTouchListener()
{
    auto listener = TouchEventListener::create();

    listener->onTouchDown = [&](TouchEvent&, WidgetPtr&) {
        this->isTouchDown = true;
        this->touchDownTime = std::time(nullptr);

        this->switchState(WidgetState::PRESSED);

        return true;
    };

    listener->onTouchMoveEnter = [&](TouchEvent&, WidgetPtr&) {
        this->switchState(WidgetState::PRESSED);
    };

    listener->onTouchMoveOut = [&](TouchEvent&, WidgetPtr&) {
        this->switchState(WidgetState::DEFAULT);
    };

    listener->onTouchUpInside = [&](TouchEvent&, WidgetPtr& widget) {
        if (this->isTouchDown) {
            if (this->onButtonClickFunc) {
                // TODO:
//                auto button = std::dynamic_pointer_cast<BaseButton>(widget);
//                this->onButtonClickFunc(button);
            }

            this->isTouchDown = false;
        }

        this->switchState(WidgetState::DEFAULT);
    };

    listener->onTouchUpOutside = [&](TouchEvent&, WidgetPtr&) {
        this->switchState(WidgetState::DEFAULT);

        this->isTouchDown = false;
    };

    listener->onTouchCancle = [&](TouchEvent&, WidgetPtr&) {
        this->switchState(WidgetState::DEFAULT);

        this->isTouchDown = false;
    };

    this->getComponent<Widget>()->addTouchListener(listener);
}

void Button::setOnClickFunc(std::function<void (ButtonPtr &)> func)
{
    this->onButtonClickFunc = func;
}

void Button::registerEvents()
{
    RegisterEvent(Events::Awake, this, &Button::onAwake);
}

void Button::copyProperties(const Button& rhs)
{
    this->isTouchDown = false;
    this->touchDownTime = 0;
}

void Button::initGraphic()
{
    ImageButtonData data = std::get<0>(datas);

    this->getComponent<Transform2D>()->setSize(data.defaultSprite->getComponent<Transform2D>()->getSize());

    getNode()->addChild(data.defaultSprite->getNode());
    getNode()->addChild(data.pressedSprite->getNode());
    getNode()->addChild(data.disabledSprite->getNode());
}

ImageButtonData::ImageButtonData(const string &texDefault, const string &texPress)
{
    ImageButtonData(texDefault, texPress, texDefault);
}

ImageButtonData::ImageButtonData(const string &texDefault, const string &texPress, const string &texDis)
{
    this->defaultSprite = CreateUIGraphicNode<Sprite>(texDefault);
    this->pressedSprite = CreateUIGraphicNode<Sprite>(texPress);
    this->disabledSprite = CreateUIGraphicNode<Sprite>(texDis);

    this->defaultSprite->rebind();
    this->pressedSprite->rebind();
    this->disabledSprite->rebind();
}

void Button::switchStateForImage(WidgetState oldState, WidgetState newState)
{
    ImageButtonData data = std::get<0>(datas);

    switch (oldState) {
    case WidgetState::PRESSED:
        data.pressedSprite->getNode()->setVisible(false);
        break;
    case WidgetState::DEFAULT:
        data.defaultSprite->getNode()->setVisible(false);
        break;
    case WidgetState::SELECTED:
        data.pressedSprite->getNode()->setVisible(false);
        break;
    case WidgetState::DISABLED:
        data.disabledSprite->getNode()->setVisible(false);
        break;
    }

    switch (newState) {
    case WidgetState::PRESSED:
        data.pressedSprite->getNode()->setVisible(true);
        break;
    case WidgetState::DEFAULT:
        data.defaultSprite->getNode()->setVisible(true);
        break;
    case WidgetState::SELECTED:
        data.pressedSprite->getNode()->setVisible(true);
        break;
    case WidgetState::DISABLED:
        data.disabledSprite->getNode()->setVisible(true);
    }
}

} // namespace ui
} // namespace re
