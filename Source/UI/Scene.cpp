#include "Scene.h"
#include "Layout/LayoutUtil.h"
#include "Layout/Screen.h"
#include "Util/ComponentFactory.h"

namespace re {
namespace ui {

COMPONENT_DEPENDENCY(Scene, Widget);

Scene::Scene()
{
}

Scene::~Scene() {

}

void Scene::onAwake() {
    this->getComponent<Transform2D>()->setSize(Screen::instance().getSize());

    this->getEntity()->addComponent<WindowManager>();
    
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

void Scene::registerEvents()
{
    RegisterEvent(Events::Awake, this, &Scene::onAwake);
    RegisterEvent(Events::Enter, this, &Scene::onEnter);
    RegisterEvent(Events::Exit, this, &Scene::onExit);
}

void Scene::onEnter() {
    MessageManager::instance().addHandler(this);
}

void Scene::onExit()
{
    MessageManager::instance().removeHandler(this);
}

} // namespace ui
} // namespace re
