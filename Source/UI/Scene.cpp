#include "Scene.h"
#include "Layout/LayoutUtil.h"
#include "Layout/Screen.h"
#include "Util/ComponentFactory.h"

namespace re {
namespace ui {

Scene::Scene()
{
}

Scene::~Scene() {

}

void Scene::init() {
    Widget::init();

    transform->setSize(Screen::getInstance().getSize());

    getEntity()->addComponent<WindowManager>();
    // TODO:
//    windowManager->start();
    
    return;
}

void Scene::handleMessage(Message *message)
{

}

// TODO: modify the dispatch of touch event

bool Scene::onBackKeyEvent() {
    auto windowManager = this->getComponent<WindowManager>();

    if (windowManager->getStackSize() > 0) {
        auto win = windowManager->getFocusedWindow();
        
        return win->onBackKeyEvent();
    } else {
        return false;
    }
}

void Scene::RegisterEvents()
{
    RegisterEvent(Events::Enter, this, &Scene::onEnter);
    RegisterEvent(Events::Exit, this, &Scene::onExit);
}

void Scene::onEnter() {
    MessageManager::getInstance().addHandler(this);
}

void Scene::onExit()
{
    MessageManager::getInstance().removeHandler(this);
}

} // namespace ui
} // namespace re
