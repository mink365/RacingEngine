#ifndef __WindowManager__
#define __WindowManager__

#include <iostream>
#include <list>
#include <string>

#include "Base/Uncopyable.h"
#include "LayerManager.h"
#include "UI/Window.h"
#include "Base/NamedFactory.h"

using namespace std;

namespace re {

typedef NamedFactory<Window> WindowFactory;
typedef std::shared_ptr<Window> WindowPtr;

class WindowManager : public Uncopyable {
public:
    WindowManager();
    
    WindowPtr pushWindow(string name);
    WindowPtr pushWindow(WindowPtr& window);
    
    WindowPtr popWindow();
    WindowPtr popWindow(WindowPtr& win);
    WindowPtr popToWindow(WindowPtr& win);
    
    void popAllWindow();
    
    WindowPtr getFocusedWindow();
    WindowPtr getWindowByName(string name);

    void setWindowFactory(WindowFactory* factory);

protected:
    virtual void addWindowToScene(WindowPtr& win) = 0;
    virtual void removeWindowFromScene(WindowPtr& win) = 0;
    virtual NodePtr getAlphaBackground() = 0;
    virtual void changeAlphaBackgroundIndex(WindowPtr& win) = 0;
    
    void changeFocusedWindow(WindowPtr& win);
    void changeFocusedWindowToStackTop();
    
    void tick();
    
    void onWindowHidden(WindowPtr& win);
    
protected:
    list<WindowPtr> windowStack;

    WindowFactory* factory;
};

inline void WindowManager::setWindowFactory(WindowFactory* factory) {
    this->factory = factory;
}

}

#endif /* defined(__WindowManager__) */
