#include "Button.h"

#include "Util/LocalTime.h"

namespace re {

BaseButton::BaseButton()
{
}

bool BaseButton::init()
{
    if (!Widget::init()) {
        return false;
    }

    this->initTouchListener();

    this->onButtonClickFunc = nullptr;

    return true;
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

    listener->onTouchDown = [&](TouchEvent& event, WidgetPtr widget) {
        this->isTouchDown = true;
        this->touchDownTime = LocalTime::getInstance().getCurrentTime();

        this->switchState(WidgetState::PRESSED);

        return true;
    };

    listener->onTouchMoveEnter = [&](TouchEvent& event, WidgetPtr widget) {
        this->switchState(WidgetState::PRESSED);
    };

    listener->onTouchMoveOut = [&](TouchEvent& event, WidgetPtr widget) {
        this->switchState(WidgetState::DEFAULT);
    };

    listener->onTouchUpInside = [&](TouchEvent& event, WidgetPtr widget) {
        if (this->isTouchDown) {
            if (this->onButtonClickFunc) {
                this->onButtonClickFunc(widget);

            }

            this->isTouchDown = false;
        }

        this->switchState(WidgetState::DEFAULT);
    };

    listener->onTouchUpOutside = [&](TouchEvent& event, WidgetPtr widget) {
        this->switchState(WidgetState::DEFAULT);

        this->isTouchDown = false;
    };

    listener->onTouchCancle = [&](TouchEvent& event, WidgetPtr widget) {
        this->switchState(WidgetState::DEFAULT);

        this->isTouchDown = false;
    };

    this->_onTouchListeners.push_back(listener);
}

bool BaseButton::onTouchEvent(TouchEvent &event)
{
    return Widget::onTouchEvent(event);
}

void BaseButton::setOnClickFunc(std::function<void (WidgetPtr &)> func)
{
    this->onButtonClickFunc = func;
}

NodePtr BaseButton::createCloneInstance() const
{
    return CreateCloneInstance<BaseButton>();
}

void BaseButton::copyProperties(const Node *node)
{
    Widget::copyProperties(node);

    const BaseButton* inst = dynamic_cast<const BaseButton*>(node);
    if (inst) {
        this->isTouchDown = false;
        this->touchDownTime = 0;
    }
}

void ImageButton::initView(const string &texDefault, const string &texPress, const string &texDis)
{
    this->defaultSprite = std::make_shared<Sprite>(texDefault);
    this->pressedSprite = std::make_shared<Sprite>(texPress);
    this->disabledSprite = std::make_shared<Sprite>(texDis);

    this->defaultSprite->rebind();
    this->pressedSprite->rebind();
    this->disabledSprite->rebind();

    this->setContentSize(this->defaultSprite->getContentSize());

    this->addChild(defaultSprite);
    this->addChild(pressedSprite);
    this->addChild(disabledSprite);

    this->defaultSprite->setVisible(true);
    this->pressedSprite->setVisible(false);
    this->disabledSprite->setVisible(false);

    this->switchState(WidgetState::DEFAULT);
}

void ImageButton::switchState(WidgetState newState)
{
    if (this->state == newState) {
        return;
    }
    WidgetState oldState = this->state;

    switch (oldState) {
    case WidgetState::PRESSED:
        pressedSprite->setVisible(false);
        break;
    case WidgetState::DEFAULT:
        defaultSprite->setVisible(false);
        break;
    case WidgetState::SELECTED:
        pressedSprite->setVisible(false);
        break;
    case WidgetState::DISABLED:
        disabledSprite->setVisible(false);
        break;
    }

    switch (newState) {
    case WidgetState::PRESSED:
        pressedSprite->setVisible(true);
        break;
    case WidgetState::DEFAULT:
        defaultSprite->setVisible(true);
        break;
    case WidgetState::SELECTED:
        pressedSprite->setVisible(true);
        break;
    case WidgetState::DISABLED:
        disabledSprite->setVisible(true);
    }
    this->state = newState;
}

NodePtr ImageButton::createCloneInstance() const
{
    return CreateCloneInstance<ImageButton>();
}

void ImageButton::copyProperties(const Node *node)
{
    BaseButton::copyProperties(node);

    const ImageButton* inst = dynamic_cast<const ImageButton*>(node);
    if (inst) {
        // TODO: get the sprite from cloned child or just not clone children?
//        this->defaultSprite = inst->defaultSprite->clone();
    }
}

NodePtr LabelButton::createCloneInstance() const
{
    return CreateCloneInstance<LabelButton>();
}

void LabelButton::copyProperties(const Node *node)
{
    ImageButton::copyProperties(node);

    const LabelButton* inst = dynamic_cast<const LabelButton*>(node);
    if (inst) {
        // TODO: get the sprite from cloned child or just not clone children?
//        this->defaultSprite = inst->defaultSprite->clone();
    }
}

}
