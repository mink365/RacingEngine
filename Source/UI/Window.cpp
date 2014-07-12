#include "Window.h"
#include "Manager/WindowManager.h"
#include "Layout/Screen.h"
#include <thread>

namespace re {

Window::Window()
: isFullScreen(true)
, isTransparent(false)
, backgroundAlpha(0.67)
, animStyle(AnimStyle::Immediately)
, manager(NULL)
{
}

bool Window::init() {
    if (!Widget::init()) {
        return false;
    }
    
    // window default is full screen
    this->setContentSize(Screen::getInstance().getSize());
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    return true;
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

void Window::setAnimationStyle(Window::AnimStyle style) {
    this->animStyle = style;
}

void Window::playShowAnim() {
    this->stopAllActions();
    
    switch (animStyle) {
        case AnimStyle::Immediately:
            this->showImmed();
            break;
        case AnimStyle::JumpOut:
        {        
            this->setScale(0.9f);
            this->setAlpha(0);
            
//            CCFiniteTimeAction *scaleAnim = CCScaleTo::create(0.20, 1.0);
//            CCFiniteTimeAction *fadeAnim = CCFadeTo::create(0.20, 255);
//            CCFiniteTimeAction *step = CCSpawn::create(scaleAnim, fadeAnim, NULL);
            
//            CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(Window::actionCallback));
//            CCFiniteTimeAction *seq = CCSequence::create(step, call, NULL);
//            this->runAction(seq);
        }
        default:
            break;
    }
}

void Window::playHideAnim() {
    this->stopAllActions();
    
    switch (animStyle) {
        case AnimStyle::Immediately:
            this->hideImmed();
            break;
        case AnimStyle::JumpOut:
        {
            this->setScale(1.0f);
            this->setAlpha(1);
//            CCFiniteTimeAction *scaleAnim = CCScaleTo::create(0.20, 0.8);
//            CCFiniteTimeAction *fadeAnim = CCFadeTo::create(0.20, 0);
//            CCFiniteTimeAction *step = CCSpawn::create(scaleAnim, fadeAnim, NULL);
            
//            CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(Window::actionCallback));
//            CCFiniteTimeAction *seq = CCSequence::create(step, call, NULL);
//            this->runAction(seq);
        }
        default:
            break;
    }
}

void Window::showImmed() {
    this->stopAllActions();
    
    IAnimationView::showImmed();
}

void Window::hideImmed() {
    this->stopAllActions();
    
    IAnimationView::hideImmed();
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

}
