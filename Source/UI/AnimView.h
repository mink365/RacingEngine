#ifndef __AnimView__
#define __AnimView__

#include <iostream>
#include <vector>

namespace re {

using namespace std;

class IAnimListener;

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

class IAnimationView {
public:
    IAnimationView();
    virtual ~IAnimationView();
    
    virtual void show();
    virtual void hide();
    
    bool isHidden();
    bool isShown();
    bool isInAnim();
    
    ViewState getViewState();
    
protected:
    void addAnimListner(IAnimListener *item);
    void removeAnimListener(IAnimListener *item);
    
    void emitAnimEvent(ViewAnimEvent event);
    
    void switchViewState(ViewState state);
    
public:
    virtual void playShowAnim();
    virtual void playHideAnim();
    
    virtual void showImmed();
    virtual void hideImmed();
    
    virtual void onViewStateChanged(ViewAnimEvent event);
    
protected:
    ViewState viewState;
    
    vector<IAnimListener*> animListeners;
};

class IAnimListener {
public:
    virtual void onShowAnimationStart(IAnimationView *view) {};
    virtual void onHideAnimationStart(IAnimationView *view) {};
    virtual void onShowAnimationEnd(IAnimationView *view) {};
    virtual void onHideAnimationEnd(IAnimationView *view) {};
    
    virtual void OnAnimationEvent(IAnimationView *view, ViewAnimEvent event) {};
};

}

#endif /* defined(__AnimView__) */
