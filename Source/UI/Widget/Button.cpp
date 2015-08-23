#include "Button.h"

#include "Util/ComponentFactory.h"
#include "Util/LocalTime.h"

namespace re {
namespace ui {

BaseButton::BaseButton()
{
}

void BaseButton::onAwake()
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

void BaseButton::setOnClickFunc(std::function<void (ButtonPtr &)> func)
{
    this->onButtonClickFunc = func;
}

void BaseButton::registerEvents()
{
    Widget::registerEvents();

    RegisterEvent(Events::Awake, this, &BaseButton::onAwake);
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

void ImageButton::init(const string &texDefault, const string &texPress)
{
    this->init(texDefault, texPress, texDefault);
}

void ImageButton::init(const string &texDefault, const string &texPress, const string &texDis)
{
    BaseButton::onAwake();

    this->defaultSprite = CreateUIGraphicNode<Sprite>(texDefault);
    this->pressedSprite = CreateUIGraphicNode<Sprite>(texPress);
    this->disabledSprite = CreateUIGraphicNode<Sprite>(texDis);

    this->defaultSprite->rebind();
    this->pressedSprite->rebind();
    this->disabledSprite->rebind();

    transform->setSize(this->defaultSprite->getComponent<Transform2D>()->getSize());

    getNode()->addChild(defaultSprite->getNode());
    getNode()->addChild(pressedSprite->getNode());
    getNode()->addChild(disabledSprite->getNode());

    defaultSprite->getNode()->setVisible(true);
    pressedSprite->getNode()->setVisible(false);
    disabledSprite->getNode()->setVisible(false);

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
        pressedSprite->getNode()->setVisible(false);
        break;
    case WidgetState::DEFAULT:
        defaultSprite->getNode()->setVisible(false);
        break;
    case WidgetState::SELECTED:
        pressedSprite->getNode()->setVisible(false);
        break;
    case WidgetState::DISABLED:
        disabledSprite->getNode()->setVisible(false);
        break;
    }

    switch (newState) {
    case WidgetState::PRESSED:
        pressedSprite->getNode()->setVisible(true);
        break;
    case WidgetState::DEFAULT:
        defaultSprite->getNode()->setVisible(true);
        break;
    case WidgetState::SELECTED:
        pressedSprite->getNode()->setVisible(true);
        break;
    case WidgetState::DISABLED:
        disabledSprite->getNode()->setVisible(true);
    }
    this->state = newState;
}

ComponentPtr ImageButton::createCloneInstance() const
{
    return CreateCloneInstance<ImageButton>();
}

void ImageButton::copyProperties(const Component *component)
{
    BaseButton::copyProperties(component);
}

ComponentPtr LabelButton::createCloneInstance() const
{
    return CreateCloneInstance<LabelButton>();
}

void LabelButton::copyProperties(const Component *component)
{
    ImageButton::copyProperties(component);
}

} // namespace ui
} // namespace re
