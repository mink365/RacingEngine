
//
//  Scene.cpp
//  MT
//
//  Created by 寇 瑞 on 7/10/13.
//
//

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

bool Scene::init() {
    if (!LogicalScene::init()) {
        return false;
    }
    
    this->setContentSize(Screen::getInstance().getSize());
    
    alphaBg = std::make_shared<Node2d>();
    alphaBg->setColor(Color::BLACK);
    
    alphaBg->setContentSize(this->getContentSize());
    alphaBg->setVisible(false);
    
    return true;
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

void Scene::update(float dt) {
    LogicalScene::update(dt);
    
    WindowManager::update(dt);
}

void Scene::onEnter() {
    LogicalScene::onEnter();
    
    this->popAllWindow();
}

void Scene::addWindowToScene(std::shared_ptr<Window> &win) {
    // TODO: zorder
    this->addChild(win);
    
    LayoutUtil::layoutParentCenter(win);
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
