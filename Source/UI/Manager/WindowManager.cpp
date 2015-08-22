#include "WindowManager.h"

#include <algorithm>
#include "Message/MessageConstant.h"
#include "Message/MessageManager.h"
#include "Scene/Node.h"
#include "UI/Layout/LayoutUtil.h"
#include "GameHub.h"

namespace re {
namespace ui {

WindowManager::WindowManager()
: factory(nullptr), alphaBg(nullptr)
{

}

void WindowManager::start()
{
    alphaBg = CreateNode2DComponent<Widget>()->getNode();
    alphaBg->getComponent<HierarchyColor>()->setColor(Color::Black);

    alphaBg->getComponent<Transform2D>()->setSize(this->getComponent<Transform2D>()->getSize());
    alphaBg->setVisible(false);
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

void WindowManager::RegisterEvents()
{
    RegisterEvent(Events::Start, this, &WindowManager::start);
    RegisterEvent(Events::Update, this, &WindowManager::tick);
    RegisterEvent(Events::Enter, this, &WindowManager::onEnter);
    RegisterEvent(Events::Exit, this, &WindowManager::onExit);
}

void WindowManager::onEnter()
{
    /*
    // TODO: easy to get the type
    auto connection = GameHub::getInstance().updateEvent.connect(slot(this, &WindowManager::tick));
    connections_.push_back(std::make_shared<ScopedConnection<void(), CollectorDefault<void>>>(connection));*/
}

void WindowManager::onExit()
{
    // auto disconnect
    connections_.clear();
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
    
    if (window->getNode()->getParent() != NULL) {
        window->getNode()->removeFromParent();
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

size_t WindowManager::getStackSize() const
{
    return this->windowStack.size();
}

void WindowManager::changeFocusedWindow(WindowPtr &win) {
    auto bg = this->getAlphaBackground();
    
    float alpha = win->getBackgroundAlpha();
        
    bg->setVisible(true);
    bg->getComponent<HierarchyColor>()->setAlpha(alpha);
        
    this->changeAlphaBackgroundIndex(win);
}

void WindowManager::changeFocusedWindowToStackTop() {
    if (!windowStack.empty()) {
        WindowPtr win = this->windowStack.back();
        
        this->changeFocusedWindow(win);
    } else {
        auto bg = this->getAlphaBackground();
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

void WindowManager::addWindowToScene(WindowPtr &win)
{
    getNode()->addChild(win->getNode());

    LayoutUtil::LayoutToParent(win->getComponent<Transform2D>(), AlignType::CENTER, AlignType::CENTER);
    win->layout();
}

void WindowManager::removeWindowFromScene(WindowPtr &win)
{
    win->getNode()->removeFromParent();
}

NodePtr WindowManager::getAlphaBackground()
{
    return this->alphaBg;
}

void WindowManager::changeAlphaBackgroundIndex(WindowPtr &win)
{
    //    this->alphaBg->setZOrder(win->getZOrder() - 1);
}

} // namespace ui
} // namespace re
