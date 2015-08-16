#ifndef __AnimView__
#define __AnimView__

#include <iostream>
#include <vector>
#include <functional>

namespace re {
namespace ui {

using namespace std;

enum class ViewState {
    IN_SHOW_ANIM,
    IN_HIDE_ANIM,
    SHOWN,
    HIDDEN,
};

enum class ViewAnimEvent {
    HIDE_START,
    SHOW_START,
    HIDE_END,
    SHOW_END,
};

class AnimationView {
public:
    typedef std::function<void(AnimationView *view, ViewAnimEvent event)> AnimationEventListener;

public:
    AnimationView();
    virtual ~AnimationView();
    
    virtual void show();
    virtual void hide();
    
    bool isHidden();
    bool isShown();
    bool isInAnim();
    
    ViewState getViewState();
    
protected:
    void addAnimListner(AnimationEventListener item);
    
    void emitAnimEvent(ViewAnimEvent event);
    
    void switchViewState(ViewState state);
    
public:
    virtual void playShowAnim();
    virtual void playHideAnim();
    
    virtual void showImmed();
    virtual void hideImmed();
    
protected:
    ViewState viewState;
    
    vector<AnimationEventListener> animListeners;
};

} // namespace ui
} // namespace re

#endif /* defined(__AnimView__) */
