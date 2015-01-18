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
    
    alphaBg = CreateNode2DComponent<Widget>()->getNode();
    alphaBg->getComponent<HierarchyColor>()->setColor(Color::Black);
    
    alphaBg->getComponent<Transform2D>()->setContentSize(this->getComponent<Transform2D>()->getContentSize());
    alphaBg->setVisible(false);
    
    return;
}

// TODO: modify the dispatch of touch event

bool Scene::onBackKeyEvent() {
    if (this->windowStack.size() > 0) {
        std::shared_ptr<Window> win = getFocusedWindow();
        
        return win->onBackKeyEvent();
    } else {
        return LogicalScene::onBackKeyEvent();
    }
}

void Scene::update()
{
    WindowManager::tick();
}

void Scene::onEnter() {
    LogicalScene::onEnter();
    
    this->popAllWindow();
}

void Scene::addWindowToScene(std::shared_ptr<Window> &win) {
    // TODO: zorder
    getNode()->addChild(win->getNode());
    
    LayoutUtil::LayoutToParentCenter(win->getComponent<Transform2D>());
    win->layout();

    return;
}

void Scene::removeWindowFromScene(std::shared_ptr<Window>& win) {
    win->getNode()->removeFromParent();
}

NodePtr Scene::getAlphaBackground() {
    return this->alphaBg;
}

void Scene::changeAlphaBackgroundIndex(std::shared_ptr<Window>& win) {
//    this->alphaBg->setZOrder(win->getZOrder() - 1);
}

}
