//
//  WindowManager.h
//  MT
//
//  Created by 寇 瑞 on 7/8/13.
//
//

#ifndef __MT__WindowManager__
#define __MT__WindowManager__

#include <iostream>
#include <list>
#include <string>

#include "Base/Uncopyable.h"
#include "LayerManager.h"
#include "UI/Window.h"
#include "ViewFactory.h"

using namespace std;

namespace re {

class WindowManager : public Uncopyable {
public:
    WindowManager();
    
    Window* pushWindow(string name);
    Window* pushWindow(Window *window);
    
    Window* popWindow();
    Window* popWindow(Window *win);
    Window* popToWindow(Window *win);
    
    void popAllWindow();
    
    Window* getFocusedWindow();
    Window* getWindowByName(string name);
    
    void setWindowFactory(IWindowFactory* factory);
    
protected:
    virtual void addWindowToScene(Window* win) = 0;
    virtual void removeWindowFromScene(Window* win) = 0;
    virtual Node2d::ptr getAlphaBackground() = 0;
    virtual void changeAlphaBackgroundIndex(Window* win) = 0;
    
    void changeFocusedWindow(Window* win);
    void changeFocusedWindowToStackTop();
    
    void update(float dt);
    
    void onWindowHidden(Window *win);
    
protected:
    list<Window*> windowStack;
    
    IWindowFactory* factory;
};

}

#endif /* defined(__MT__WindowManager__) */
