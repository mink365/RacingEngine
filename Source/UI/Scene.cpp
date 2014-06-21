
//
//  Scene.cpp
//  MT
//
//  Created by 寇 瑞 on 7/10/13.
//
//

#include "Scene.h"
#include "Layout/LayoutUtil.h"

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
    
//    CCDirector* pDirector = CCDirector::sharedDirector();
//    this->setContentSize(pDirector->getWinSize());
    
//    alphaBg = ColorQuadNode::create();
//    alphaBg->setColor(ccBLACK);
    
//    alphaBg->setContentSize(this->getContentSize());
//    alphaBg->setVisible(false);
//    this->addChild(alphaBg, 5);
//    alphaBg->retain();
    
    return true;
}

//bool Scene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
//    if (this->windowStack.size() > 0) {
//        // dispatch to top window
//        Window *win = getFocusedWindow();
//        return win->ccTouchBegan(pTouch, pEvent);
//    } else {
//        return LogicalScene::ccTouchBegan(pTouch, pEvent);
//    }
//}

bool Scene::onBackKeyEvent() {
    if (this->windowStack.size() > 0) {
        Window* win = getFocusedWindow();
        
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

void Scene::addWindowToScene(Window *win) {    
//    this->addChild(win, this->windowStack.size() + 10);
    
//    LayoutUtil::layoutParentCenter(win);
    win->layout();
}

void Scene::removeWindowFromScene(Window *win) {
    win->removeFromParent();
}

Node2d::ptr Scene::getAlphaBackground() {
    return this->alphaBg;
}

void Scene::changeAlphaBackgroundIndex(Window* win) {
//    this->alphaBg->setZOrder(win->getZOrder() - 1);
}

}
