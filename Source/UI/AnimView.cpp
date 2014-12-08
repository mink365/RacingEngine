
#include "AnimView.h"

#include <algorithm>
#include <thread>

namespace re {

AnimationView::AnimationView()
: viewState(ViewState::HIDDEN)
{
}

AnimationView::~AnimationView() {

}

void AnimationView::show() {
    if (this->viewState == ViewState::SHOWN
        || this->viewState == ViewState::IN_SHOW_ANIM) {
        return;
    }
    
    this->switchViewState(ViewState::IN_SHOW_ANIM);
}

void AnimationView::hide() {
    if (this->viewState == ViewState::HIDDEN
        || this->viewState == ViewState::IN_HIDE_ANIM) {
        return;
    }
    
    this->switchViewState(ViewState::IN_HIDE_ANIM);
}

void AnimationView::playShowAnim() {
    this->showImmed();
}

void AnimationView::playHideAnim() {
    this->hideImmed();
}

void AnimationView::showImmed() {
    this->switchViewState(ViewState::SHOWN);
}

void AnimationView::hideImmed() {
    this->switchViewState(ViewState::HIDDEN);
}

bool AnimationView::isHidden() {
    return this->viewState == ViewState::HIDDEN;
}

bool AnimationView::isShown() {
    return viewState == ViewState::SHOWN;
}

bool AnimationView::isInAnim() {
    return viewState == ViewState::IN_HIDE_ANIM || viewState == ViewState::IN_SHOW_ANIM;
}

ViewState AnimationView::getViewState() {
    return viewState;
}

void AnimationView::switchViewState(ViewState newState) {
    if (this->viewState == newState) {
        return;
    }
        
    ViewState oldState = viewState;
    this->viewState = newState;
    
    switch (newState) {
        case ViewState::SHOWN:
            this->emitAnimEvent(ViewAnimEvent::SHOW_END);
            break;
        case ViewState::HIDDEN:
            this->emitAnimEvent(ViewAnimEvent::HIDE_END);
            break;
        case ViewState::IN_HIDE_ANIM:
            this->emitAnimEvent(ViewAnimEvent::HIDE_START);
            
            this->playHideAnim();
            break;
        case ViewState::IN_SHOW_ANIM:
            this->emitAnimEvent(ViewAnimEvent::SHOW_START);
            
            this->playShowAnim();
            break;
            
        default:
            break;
    }
}

void AnimationView::addAnimListner(AnimationEventListener item) {
    this->animListeners.push_back(item);
}

void AnimationView::emitAnimEvent(ViewAnimEvent event) {
    for (auto listener : animListeners) {
        listener(this, event);
    }
}

}
