#include "Scene.h"
#include "Layout/LayoutUtil.h"
#include "Layout/Screen.h"
#include "Util/ComponentFactory.h"

namespace re {

Scene::Scene()
{
}

Scene::~Scene() {

}

void Scene::init() {
    Widget::init();

    transform->setContentSize(Screen::getInstance().getSize());

    auto windowManager = std::make_shared<WindowManager>();
    getNode()->addComponent(windowManager);
    windowManager->start();
    
    return;
}

void Scene::handleMessage(Message *message)
{

}

// TODO: modify the dispatch of touch event

bool Scene::onBackKeyEvent() {
    auto windowManager = this->getComponent<WindowManager>();

    if (windowManager->getStackSize() > 0) {
        std::shared_ptr<Window> win = windowManager->getFocusedWindow();
        
        return win->onBackKeyEvent();
    } else {
        return false;
    }
}

void Scene::onEnter() {
    Widget::onEnter();

    MessageManager::getInstance().addHandler(this);
}

void Scene::onExit()
{
    MessageManager::getInstance().removeHandler(this);

    Widget::onExit();
}

}
