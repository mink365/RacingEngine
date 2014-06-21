#ifndef WINDOW_H
#define WINDOW_H

#include "Widget.h"
#include "AnimView.h"
#include "IBackKeyHandler.h"

namespace re {

class WindowManager;

class Window : public Widget, public IAnimationView, public IBackKeyHandler
{
    friend class WindowManager;
    
public:
    enum class AnimStyle {
        Immediately,
        JumpOut,
    };
    
public:
    Window();
    
    bool init();
    
public:    
    bool getFullScreenState();
    bool getTransparentState();
    float getBackgroundAlpha();
    
    void setAnimationStyle(AnimStyle style);
    
    /*
     * 从窗口堆栈弹出自己
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
    bool isFullScreen;
    bool isTransparent;
    
    float backgroundAlpha;
    
    AnimStyle animStyle;
    
    WindowManager* manager;
};

}

#endif // WINDOW_H
