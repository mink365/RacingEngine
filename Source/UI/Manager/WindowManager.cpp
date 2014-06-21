//
//  WindowManager.cpp
//  MT
//
//  Created by 寇 瑞 on 7/8/13.
//
//

#include "WindowManager.h"

#include <algorithm>
#include "Message/MessageConstant.h"
#include "Message/MessageManager.h"
#include "ViewFactory.h"

namespace re {

WindowManager::WindowManager()
: factory(nullptr)
{

}

void WindowManager::setWindowFactory(IWindowFactory *factory) {
    assert(this->factory == nullptr);
    
    this->factory = factory;
}

Window* WindowManager::getFocusedWindow() {
    if (windowStack.empty()) {
        return NULL;
    } else {
        return windowStack.back();
    }
}

Window* WindowManager::getWindowByName(string name) {
    auto query = find_if(windowStack.begin(), windowStack.end(), [name] (Window* window) {
        return window->getName() == name;
    });
    if (query != windowStack.end()) {
        return *query;
    } else {
        return NULL;
    }
}

Window* WindowManager::pushWindow(string name) {
    Window *win = factory->getWindow(name);
    assert(win);
    
    this->pushWindow(win);
    
    return win;
}

Window* WindowManager::pushWindow(Window *window) {
    auto iter = find(windowStack.begin(), windowStack.end(), window);
    
    if (iter != windowStack.end()) {
        Window* win = *iter;
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
    
    MessageManager::getInstance()->sendMessage(MessageConstant::MessageType::WINDOW_MESSAGE,
                                               MessageConstant::WindowMessage::PUSH_WINDOW,
                                               make_shared<string>(window->getName()));
    
    this->changeFocusedWindowToStackTop();
    
    return window;
}

Window* WindowManager::popWindow() {
    assert(!windowStack.empty());
    
    Window* win = this->windowStack.back();
    
    popWindow(win);
    
    return win;
}

Window* WindowManager::popWindow(Window *win) {
    assert(!windowStack.empty());
    
    auto iter = find(windowStack.begin(), windowStack.end(), win);
    assert(iter != windowStack.end());
    
    win->hide();
    
    MessageManager::getInstance()->sendMessage(MessageConstant::MessageType::WINDOW_MESSAGE,
                                               MessageConstant::WindowMessage::POP_WINDOW,
                                               make_shared<string>(win->getName()));
    
    return win;
}

Window* WindowManager::popToWindow(Window *win) {
    assert(!windowStack.empty());
    
    while (windowStack.back() != win) {
        Window *upper = windowStack.back();
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
        Window* win = this->windowStack.back();
        
        this->onWindowHidden(win);
    }
}

void WindowManager::changeFocusedWindow(Window *win) {
    std::shared_ptr<Node2d> bg = this->getAlphaBackground();
    
    if (win != NULL) {
        float alpha = win->getBackgroundAlpha();
        
        bg->setVisible(true);
        bg->setAlpha(alpha);
        
        this->changeAlphaBackgroundIndex(win);
    } else {
        bg->setVisible(false);
    }
}

void WindowManager::changeFocusedWindowToStackTop() {
    if (!windowStack.empty()) {
        Window *win = this->windowStack.back();
        
        this->changeFocusedWindow(win);
    } else {
        this->changeFocusedWindow(NULL);
    }
}

void WindowManager::update(float dt) {
    auto iter = find_if(windowStack.begin(), windowStack.end(), [](Window *win){
        return win->isHidden();
    });
    
    if (iter != windowStack.end()) {
        this->onWindowHidden(*iter);
    }
}

void WindowManager::onWindowHidden(Window *win) {
    this->removeWindowFromScene(win);
    
    auto iter = find(windowStack.begin(), windowStack.end(), win);
    if (iter != windowStack.end()) {
        windowStack.erase(iter);
    }
    win->manager = NULL;
    
    this->changeFocusedWindowToStackTop();
}

}
