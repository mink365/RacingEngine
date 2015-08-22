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
typedef SharedPtr<Window> WindowPtr;

class WindowManager : public Component {
public:
    WindowManager();

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

public:
    void RegisterEvents();

protected:
    void onAwake();
    void onEnter();
    void onExit();
    void onUpdate();

    void addWindowToScene(WindowPtr& win);
    void removeWindowFromScene(WindowPtr& win);
    NodePtr getAlphaBackground();
    void changeAlphaBackgroundIndex(WindowPtr& win);
    
    void changeFocusedWindow(WindowPtr& win);
    void changeFocusedWindowToStackTop();
    
    void onWindowHidden(WindowPtr& win);
    
protected:
    std::list<WindowPtr> windowStack;

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
