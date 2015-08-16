#ifndef __WindowManager__
#define __WindowManager__

#include <iostream>
#include <list>
#include <string>

#include "Base/Uncopyable.h"
#include "LayerManager.h"
#include "UI/Window.h"
#include "Base/NamedFactory.h"

namespace re {
namespace ui {

typedef NamedFactory<Window> WindowFactory;
typedef std::shared_ptr<Window> WindowPtr;

class WindowManager : public Component {
public:
    WindowManager();

    virtual void start();
    
    WindowPtr pushWindow(string name);
    WindowPtr pushWindow(WindowPtr& window);
    
    WindowPtr popWindow();
    WindowPtr popWindow(WindowPtr& win);
    WindowPtr popToWindow(WindowPtr& win);
    
    void popAllWindow();
    
    size_t getStackSize() const;
    WindowPtr getFocusedWindow();
    WindowPtr getWindowByName(string name);

    void setWindowFactory(WindowFactory* factory);

protected:
    virtual void onEnter();
    virtual void onExit();

    virtual void addWindowToScene(WindowPtr& win);
    virtual void removeWindowFromScene(WindowPtr& win);
    virtual NodePtr getAlphaBackground();
    virtual void changeAlphaBackgroundIndex(WindowPtr& win);
    
    void changeFocusedWindow(WindowPtr& win);
    void changeFocusedWindowToStackTop();
    
    void tick();
    
    void onWindowHidden(WindowPtr& win);
    
protected:
    list<WindowPtr> windowStack;

    WindowFactory* factory;

    NodePtr alphaBg;

    std::vector<std::shared_ptr<ScopedConnection<void(), CollectorDefault<void>>>> connections_;
};

inline void WindowManager::setWindowFactory(WindowFactory* factory) {
    this->factory = factory;
}

} // namespace ui
} // namespace re

#endif /* defined(__WindowManager__) */
