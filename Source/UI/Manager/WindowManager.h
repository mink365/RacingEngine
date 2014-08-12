#ifndef __WindowManager__
#define __WindowManager__

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
    
    std::shared_ptr<Window> pushWindow(string name);
    std::shared_ptr<Window> pushWindow(std::shared_ptr<Window>& window);
    
    std::shared_ptr<Window> popWindow();
    std::shared_ptr<Window> popWindow(std::shared_ptr<Window>& win);
    std::shared_ptr<Window> popToWindow(std::shared_ptr<Window>& win);
    
    void popAllWindow();
    
    std::shared_ptr<Window> getFocusedWindow();
    std::shared_ptr<Window> getWindowByName(string name);
    
    void setWindowFactory(std::shared_ptr<IWindowFactory>& factory);
    
protected:
    virtual void addWindowToScene(std::shared_ptr<Window>& win) = 0;
    virtual void removeWindowFromScene(std::shared_ptr<Window>& win) = 0;
    virtual Node2d::ptr getAlphaBackground() = 0;
    virtual void changeAlphaBackgroundIndex(std::shared_ptr<Window>& win) = 0;
    
    void changeFocusedWindow(std::shared_ptr<Window>& win);
    void changeFocusedWindowToStackTop();
    
    void tick();
    
    void onWindowHidden(std::shared_ptr<Window>& win);
    
protected:
    list<std::shared_ptr<Window>> windowStack;
    
    std::shared_ptr<IWindowFactory> factory;
};

}

#endif /* defined(__WindowManager__) */
