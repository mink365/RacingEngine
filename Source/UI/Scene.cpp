#include "Scene.h"
#include "Layout/LayoutUtil.h"
#include "Layout/Screen.h"
#include "Base/Node2d.h"

namespace re {

Scene::Scene()
{
}

Scene::~Scene() {

}

void Scene::init() {
    this->setContentSize(Screen::getInstance().getSize());
    
    alphaBg = std::make_shared<Node2d>();
    alphaBg->setColor(Color::Black);
    
    alphaBg->setContentSize(this->getContentSize());
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

void Scene::updateSelf(float delta)
{
    WindowManager::tick();
}

void Scene::onEnter() {
    LogicalScene::onEnter();
    
    this->popAllWindow();
}

void Scene::addWindowToScene(std::shared_ptr<Window> &win) {
    // TODO: zorder
    this->addChild(win);
    
    LayoutUtil::LayoutToParentCenter(win);
    win->layout();

    return;
}

void Scene::removeWindowFromScene(std::shared_ptr<Window>& win) {
    win->removeFromParent();
}

Node2d::ptr Scene::getAlphaBackground() {
    return this->alphaBg;
}

void Scene::changeAlphaBackgroundIndex(std::shared_ptr<Window>& win) {
//    this->alphaBg->setZOrder(win->getZOrder() - 1);
}

}
