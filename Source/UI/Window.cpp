#include "Window.h"
#include "Manager/WindowManager.h"
#include "Layout/Screen.h"
#include "Tween.h"
#include "Timeline.h"
#include "TweenManager.h"
#include "Util/PredefineTweenAccessor.h"
#include "GameHub.h"

using namespace TweenEngine;

namespace re {

Window::Window()
: isFullScreen(true)
, isTransparent(false)
, backgroundAlpha(0.67)
, manager(NULL)
{
    this->showAnimFunc = nullptr;
    this->hideAnimFunc = nullptr;
}

void Window::init() {
    // window default is full screen
    this->setContentSize(Screen::getInstance().getSize());
    
    this->setAnchorPoint(Vec2(0.5, 0.5));

    this->initAnimFunc();
}

bool Window::getFullScreenState() {
    return isFullScreen;
}

bool Window::getTransparentState() {
    return isTransparent;
}

float Window::getBackgroundAlpha() {
    return backgroundAlpha;
}


void Window::playShowAnim() {
    this->stopAllActions();
    
    this->showAnimFunc();
}

void Window::playHideAnim() {
    this->stopAllActions();
    
    this->hideAnimFunc();
}

void Window::showImmed() {
    this->stopAllActions();
    
    AnimationView::showImmed();
}

void Window::hideImmed() {
    this->stopAllActions();
    
    AnimationView::hideImmed();
}

void Window::actionCallback() {
    if (this->viewState == ViewState::IN_SHOW_ANIM) {
        this->switchViewState(ViewState::SHOWN);
    } else {
        this->switchViewState(ViewState::HIDDEN);
    }
}

void Window::popFromWindowManager() {
    if (manager == NULL) {
        return;
    }
    
    auto thisPtr = this->shared_from_this();
    auto win = std::dynamic_pointer_cast<Window>(thisPtr);

    manager->popWindow(win);
}

bool Window::onBackKeyEvent() {
    this->popFromWindowManager();
    
    return true;
}

void Window::initAnimFunc()
{
    auto ptr = std::static_pointer_cast<Window>(this->shared_from_this());

    std::function<void(TweenCallbackType type, BaseTween *source)> animCallback = [=](TweenCallbackType type, BaseTween *source) {
        ptr->actionCallback();
    };

    this->showAnimFunc = [=]() {
        Timeline::createSequence()
            .beginParallel()
                .push(Tween::set(ptr, FlatNodeAccessor::SCALE).target(0.4f))
                .push(Tween::set(ptr, FlatNodeAccessor::ALPHA).target(0.6f))
            .end()

            .beginParallel()
                .push(Tween::to(ptr, FlatNodeAccessor::SCALE, 0.7).target(1.0f))
                .push(Tween::to(ptr, FlatNodeAccessor::ALPHA, 0.7).target(1.0f))
            .end()

            .setCallback(animCallback)
            .setCallbackTriggers(TweenCallbackType::COMPLETE)
            .start(GameHub::getInstance().GetTweenManager());
    };

    this->hideAnimFunc = [=]() {
        Timeline::createSequence()
            .beginParallel()
                .push(Tween::set(ptr, FlatNodeAccessor::SCALE).target(1.0f))
                .push(Tween::set(ptr, FlatNodeAccessor::ALPHA).target(1.0f))
            .end()

            .beginParallel()
                .push(Tween::to(ptr, FlatNodeAccessor::SCALE, 0.7).target(0.4))
                .push(Tween::to(ptr, FlatNodeAccessor::ALPHA, 0.7).target(0.6))
            .end()

            .setCallback(animCallback)
            .setCallbackTriggers(TweenCallbackType::COMPLETE)
            .start(GameHub::getInstance().GetTweenManager());
    };
}

}
