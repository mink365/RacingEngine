#include "Button.h"

#include "Util/ComponentFactory.h"
#include "UI/Base/Image.h"
#include "Texture/Frame/TextureFrame.h"
#include "Base/ECS/System.h"

namespace re {
namespace ui {

COMPONENT_DEPENDENCY(Button, Node, ui::Transform2D, ui::HierarchyColor, ui::LayoutElement, ui::Widget, ui::Image);

Button::Button()
{
}

void Button::onAwake()
{
    this->initTouchListener();

    this->onButtonClickFunc = nullptr;

    this->initGraphic();
    this->switchState(WidgetState::DEFAULT);
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
                ButtonPtr button = widget->getComponent<Button>();
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

    this->datas = rhs.datas;
}

void Button::initGraphic()
{
    auto& data = getData<ButtonType::SpriteSwap>();

    this->getComponent<Transform2D>()->setSize(data.defaultSprite->getOriginalSize());
}

ImageButtonData::ImageButtonData(const string &texDefault, const string &texPress)
{
    ImageButtonData(texDefault, texPress, texDefault);
}

ImageButtonData::ImageButtonData(const string &texDefault, const string &texPress, const string &texDis)
{
    this->defaultSprite = TextureFrameManager::instance().GetResource(texDefault);
    this->pressedSprite = TextureFrameManager::instance().GetResource(texPress);
    this->disabledSprite = TextureFrameManager::instance().GetResource(texDis);
}

void Button::switchStateForImage(WidgetState oldState, WidgetState newState)
{
    ImageButtonData data = getData<ButtonType::SpriteSwap>();

    auto sprite = this->getComponent<ui::Image>();

    switch (newState) {
    case WidgetState::DEFAULT:
        sprite->setFrame(data.defaultSprite);
        break;
    case WidgetState::PRESSED:
        sprite->setFrame(data.pressedSprite);
        break;
    case WidgetState::SELECTED:
        sprite->setFrame(data.pressedSprite);
        break;
    case WidgetState::DISABLED:
        sprite->setFrame(data.disabledSprite);
        break;
    case WidgetState::INIT:
        break;
    }
}

} // namespace ui
} // namespace re
