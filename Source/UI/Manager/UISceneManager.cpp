//
//  SceneManager.cpp
//  MT
//
//  Created by 寇 瑞 on 7/23/13.
//
//

#include "UISceneManager.h"

//#include "WaitingWindowController.h"
#include "UI/Layout/LayoutUtil.h"

namespace re {
    namespace ui {

UISceneManager::UISceneManager()
: factory(nullptr)
, isKeyBackActive(false)
{
}

bool UISceneManager::init() {
    if (!LogicalScene::init()) {
        return false;
    }
    isKeyBackActive = true;
    
//    CCDirector* pDirector = CCDirector::sharedDirector();
//    this->setContentSize(pDirector->getWinSize());
    
    addWidgets();
    
    return true;
}

void UISceneManager::setSceneFactory(ISceneFactory *factory) {
    assert(this->factory == nullptr);
    
    this->factory = factory;
}

void UISceneManager::handleMessage(Message *message) {
//    WaitingWindowController::getInstance()->checkMessage(message);
    
    LogicalScene::handleMessage(message);
}

void UISceneManager::addWidgets() {
//    this->addChild(guidLayer, 1);
//    LayoutUtil::layoutParentCenter(guidLayer);
}

void UISceneManager::updateSelf(float delta) {
//    WaitingWindowController::getInstance()->update();
    LayerManager::update(delta);
}

Scene* UISceneManager::createLayer(const std::string &name) {
    Scene* scene = this->factory->getSceneByName(name);
    
    return scene;
}

void UISceneManager::addLayerToScene(Scene* node) {
//    this->addChild(node);
    
//    LayoutUtil::layoutParentCenter(node);
}

void UISceneManager::removeLayerFromScene(Scene *node) {
    string name = node->getName();
//    node->removeFromParent();
}

Scene* UISceneManager::getDefaultLayer() {
    return NULL;
}

void UISceneManager::onEnter() {
//    this->scheduleUpdate();
//    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0 , true);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    cocos2d::CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);
//#endif
    LogicalScene::onEnter();
}

void UISceneManager::onExit() {
//    this->unscheduleUpdate();
//    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    cocos2d::CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate(this);
//#endif
    LogicalScene::onExit();
}

void UISceneManager::keyBackClicked() {
    if (!isKeyBackActive) {
        return;
    }
    Scene* scene = this->getLastLayer();
    
    if (scene) {
        scene->onBackKeyEvent();
    }
}

void UISceneManager::setKeyBackActive(bool active){
    isKeyBackActive = active;
}

    }
}
