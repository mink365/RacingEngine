#include "Button.h"

#include "Util/LocalTime.h"

namespace re {

BaseButton::BaseButton()
{
}

void BaseButton::init()
{
    this->initTouchListener();

    this->onButtonClickFunc = nullptr;
}

void BaseButton::switchState(WidgetState newState)
{
    if (this->state == newState) {
        return;
    }

    this->state = newState;
}

void BaseButton::initTouchListener()
{
    auto listener = TouchEventListener::create();

    listener->onTouchDown = [&](TouchEvent&, WidgetPtr&) {
        this->isTouchDown = true;
        this->touchDownTime = LocalTime::getInstance().getCurrentTime();

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
                auto button = std::dynamic_pointer_cast<BaseButton>(widget);
                this->onButtonClickFunc(button);
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

    this->_onTouchListeners.push_back(listener);
}

bool BaseButton::onTouchEvent(TouchEvent &event)
{
    return Widget::onTouchEvent(event);
}

void BaseButton::setOnClickFunc(std::function<void (ButtonPtr &)> func)
{
    this->onButtonClickFunc = func;
}

ComponentPtr BaseButton::createCloneInstance() const
{
    return CreateCloneInstance<BaseButton>();
}

void BaseButton::copyProperties(const Component *component)
{
    Widget::copyProperties(component);

    const BaseButton* inst = dynamic_cast<const BaseButton*>(component);
    if (inst) {
        this->isTouchDown = false;
        this->touchDownTime = 0;
    }
}

void re::ImageButton::init(const string &texDefault, const string &texPress)
{
    this->init(texDefault, texPress, texDefault);
}

void ImageButton::init(const string &texDefault, const string &texPress, const string &texDis)
{
    BaseButton::init();

    this->defaultSprite = Create<Sprite>(texDefault);
    this->pressedSprite = Create<Sprite>(texPress);
    this->disabledSprite = Create<Sprite>(texDis);

    this->defaultSprite->rebind();
    this->pressedSprite->rebind();
    this->disabledSprite->rebind();

    transform->setContentSize(this->defaultSprite->getComponent<Transform2D>()->getContentSize());

//    this->addChild(defaultSprite);
//    this->addChild(pressedSprite);
//    this->addChild(disabledSprite);

//    this->defaultSprite->setVisible(true);
//    this->pressedSprite->setVisible(false);
//    this->disabledSprite->setVisible(false);

    this->switchState(WidgetState::DEFAULT);
}

void ImageButton::switchState(WidgetState newState)
{
    if (this->state == newState) {
        return;
    }
    WidgetState oldState = this->state;

//    switch (oldState) {
//    case WidgetState::PRESSED:
//        pressedSprite->setVisible(false);
//        break;
//    case WidgetState::DEFAULT:
//        defaultSprite->setVisible(false);
//        break;
//    case WidgetState::SELECTED:
//        pressedSprite->setVisible(false);
//        break;
//    case WidgetState::DISABLED:
//        disabledSprite->setVisible(false);
//        break;
//    }

//    switch (newState) {
//    case WidgetState::PRESSED:
//        pressedSprite->setVisible(true);
//        break;
//    case WidgetState::DEFAULT:
//        defaultSprite->setVisible(true);
//        break;
//    case WidgetState::SELECTED:
//        pressedSprite->setVisible(true);
//        break;
//    case WidgetState::DISABLED:
//        disabledSprite->setVisible(true);
//    }
    this->state = newState;
}

ComponentPtr ImageButton::createCloneInstance() const
{
    return CreateCloneInstance<ImageButton>();
}

void ImageButton::copyProperties(const Component *component)
{
    BaseButton::copyProperties(component);

    const ImageButton* inst = dynamic_cast<const ImageButton*>(component);
    if (inst) {
        // TODO: get the sprite from cloned child or just not clone children?
//        this->defaultSprite = inst->defaultSprite->clone();
    }
}

ComponentPtr LabelButton::createCloneInstance() const
{
    return CreateCloneInstance<LabelButton>();
}

void LabelButton::copyProperties(const Component *component)
{
    ImageButton::copyProperties(component);

    const LabelButton* inst = dynamic_cast<const LabelButton*>(component);
    if (inst) {
        // TODO: get the sprite from cloned child or just not clone children?
//        this->defaultSprite = inst->defaultSprite->clone();
    }
}

}
