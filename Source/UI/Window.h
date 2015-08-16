#ifndef WINDOW_H
#define WINDOW_H

#include "Widget.h"
#include "AnimView.h"
#include "IBackKeyHandler.h"

namespace re {
namespace ui {

class WindowManager;

class Window : public Widget, public AnimationView, public IBackKeyHandler
{
    friend class WindowManager;
    
public:
    Window();
    
    void init();
    
public:    
    bool getFullScreenState();
    bool getTransparentState();
    float getBackgroundAlpha();
    
    /*
     * pop self from the window stack
     */
    void popFromWindowManager();
    
    void actionCallback();
    
public:
    void playShowAnim();
    void playHideAnim();
    
    void showImmed();
    void hideImmed();
    
    virtual bool onBackKeyEvent();

protected:
    virtual void initAnimFunc();
        
protected:
    bool isFullScreen;
    bool isTransparent;
    
    float backgroundAlpha;

    std::function<void()> showAnimFunc;
    std::function<void()> hideAnimFunc;
    
    WindowManager* manager;
};

} // namespace ui
} // namespace re

#endif // WINDOW_H
