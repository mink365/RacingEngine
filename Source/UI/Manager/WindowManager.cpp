#include "WindowManager.h"

#include <algorithm>
#include "Message/MessageConstant.h"
#include "Message/MessageManager.h"

namespace re {

WindowManager::WindowManager()
: factory(nullptr)
{

}

WindowPtr WindowManager::getFocusedWindow() {
    if (windowStack.empty()) {
        return NULL;
    } else {
        return windowStack.back();
    }
}

WindowPtr WindowManager::getWindowByName(string name) {
    auto query = find_if(windowStack.begin(), windowStack.end(), [name] (WindowPtr window) {
        return window->getName() == name;
    });
    if (query != windowStack.end()) {
        return *query;
    } else {
        return NULL;
    }
}

WindowPtr WindowManager::pushWindow(string name) {
    WindowPtr win = factory->createInstance(name);
    assert(win);
    
    this->pushWindow(win);
    
    return win;
}

WindowPtr WindowManager::pushWindow(WindowPtr &window) {
    auto iter = find(windowStack.begin(), windowStack.end(), window);
    
    if (iter != windowStack.end()) {
        WindowPtr win = *iter;
        win->hideImmed();
        onWindowHidden(win);
    }
    
    if (window->getParent() != NULL) {
        window->removeFromParent();
    }
    
    this->addWindowToScene(window);
    
    this->windowStack.push_back(window);
    window->manager = this;
    window->show();
    
    MessageManager::getInstance().sendMessage(MessageConstant::MessageType::WINDOW_MESSAGE,
                                               MessageConstant::WindowMessage::PUSH_WINDOW,
                                               make_shared<string>(window->getName()));
    
    this->changeFocusedWindowToStackTop();
    
    return window;
}

WindowPtr WindowManager::popWindow() {
    assert(!windowStack.empty());
    
    WindowPtr win = this->windowStack.back();
    
    popWindow(win);
    
    return win;
}

WindowPtr WindowManager::popWindow(WindowPtr &win) {
    assert(!windowStack.empty());
    
    auto iter = find(windowStack.begin(), windowStack.end(), win);
    assert(iter != windowStack.end());
    
    win->hide();
    
    MessageManager::getInstance().sendMessage(MessageConstant::MessageType::WINDOW_MESSAGE,
                                               MessageConstant::WindowMessage::POP_WINDOW,
                                               make_shared<string>(win->getName()));
    
    return win;
}

WindowPtr WindowManager::popToWindow(WindowPtr &win) {
    assert(!windowStack.empty());
    
    while (windowStack.back() != win) {
        WindowPtr upper = windowStack.back();
        upper->hideImmed();
        
        onWindowHidden(upper);
        if (windowStack.size() == 0) {
            break;
        }
    }
    
    win->show();
    
    this->changeFocusedWindowToStackTop();
    
    if (windowStack.empty()) {
        return NULL;
    } else {
        return windowStack.back();
    }
}

void WindowManager::popAllWindow() {
    while (!windowStack.empty()) {
        WindowPtr win = this->windowStack.back();
        
        this->onWindowHidden(win);
    }
}

void WindowManager::changeFocusedWindow(WindowPtr &win) {
    std::shared_ptr<Node2d> bg = this->getAlphaBackground();
    
    float alpha = win->getBackgroundAlpha();
        
    bg->setVisible(true);
    bg->setAlpha(alpha);
        
    this->changeAlphaBackgroundIndex(win);
}

void WindowManager::changeFocusedWindowToStackTop() {
    if (!windowStack.empty()) {
        WindowPtr win = this->windowStack.back();
        
        this->changeFocusedWindow(win);
    } else {
        std::shared_ptr<Node2d> bg = this->getAlphaBackground();
        bg->setVisible(false);
    }
}

void WindowManager::tick() {
    auto iter = find_if(windowStack.begin(), windowStack.end(), [](WindowPtr win){
        return win->isHidden();
    });
    
    if (iter != windowStack.end()) {
        this->onWindowHidden(*iter);
    }
}

void WindowManager::onWindowHidden(WindowPtr &win) {
    this->removeWindowFromScene(win);
    
    auto iter = find(windowStack.begin(), windowStack.end(), win);
    if (iter != windowStack.end()) {
        windowStack.erase(iter);
    }
    win->manager = NULL;
    
    this->changeFocusedWindowToStackTop();
}

}
