
#include "AnimView.h"

#include <algorithm>
#include <thread>

namespace re {

IAnimationView::IAnimationView()
: viewState(ViewState::HIDDEN)
{
}

IAnimationView::~IAnimationView() {

}

void IAnimationView::show() {
    if (this->viewState == ViewState::SHOWN
        || this->viewState == ViewState::IN_SHOW_ANIM) {
        return;
    }
    
    this->switchViewState(ViewState::IN_SHOW_ANIM);
}

void IAnimationView::hide() {
    if (this->viewState == ViewState::HIDDEN
        || this->viewState == ViewState::IN_HIDE_ANIM) {
        return;
    }
    
    this->switchViewState(ViewState::IN_HIDE_ANIM);
}

void IAnimationView::playShowAnim() {
    this->showImmed();
}

void IAnimationView::playHideAnim() {
    this->hideImmed();
}

void IAnimationView::showImmed() {    
    this->switchViewState(ViewState::SHOWN);
}

void IAnimationView::hideImmed() {
    this->switchViewState(ViewState::HIDDEN);
}

bool IAnimationView::isHidden() {
    return this->viewState == ViewState::HIDDEN;
}

bool IAnimationView::isShown() {
    return viewState == ViewState::SHOWN;
}

bool IAnimationView::isInAnim() {
    return viewState == ViewState::IN_HIDE_ANIM || viewState == ViewState::IN_SHOW_ANIM;
}

ViewState IAnimationView::getViewState() {
    return viewState;
}

void IAnimationView::switchViewState(ViewState newState) {
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

void IAnimationView::addAnimListner(IAnimListener *item) {
    this->animListeners.push_back(item);
}

void IAnimationView::removeAnimListener(IAnimListener *item) {
    auto iter = find(animListeners.begin(), animListeners.end(), item);
    
    if (iter != animListeners.end()) {
        this->animListeners.erase(iter);
    }
}

void IAnimationView::emitAnimEvent(ViewAnimEvent event) {
    this->onViewStateChanged(event);
    
    for (auto listener : animListeners) {
        switch (event) {
            case ViewAnimEvent::SHOW_START:
                listener->onShowAnimationStart(this);
                break;
            case ViewAnimEvent::SHOW_END:
                listener->onShowAnimationEnd(this);
                break;
            case ViewAnimEvent::HIDE_START:
                listener->onHideAnimationStart(this);
                break;
            case ViewAnimEvent::HIDE_END:
                listener->onHideAnimationEnd(this);
                break;
                
            default:
                break;
        }
        
        listener->OnAnimationEvent(this, event);
    }
}

void IAnimationView::onViewStateChanged(ViewAnimEvent event) {
    
}

}
