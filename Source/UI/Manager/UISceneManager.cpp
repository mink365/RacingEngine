//
//  SceneManager.cpp
//  MT
//
//  Created by 寇 瑞 on 7/23/13.
//
//

#include "UISceneManager.h"

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

    this->setContentSize(Screen::getInstance().getSize());

    isKeyBackActive = true;
    
    addWidgets();
    
    return true;
}

void UISceneManager::setSceneFactory(std::shared_ptr<ISceneFactory> &factory) {
    assert(this->factory == nullptr);
    
    this->factory = factory;
}

void UISceneManager::handleMessage(Message *message) {
//    LogicalScene::handleMessage(message);

    if (message->getType() == MessageConstant::MessageType::TOUCHSCREEN_MESSAGE) {
        TouchEvent* event = static_cast<TouchEvent*>(message->getData());

        event->setCurrPoint(event->getPoint() / Screen::getInstance().getFinalScale());
        this->dispatchTouchEvent(*event);
    }
}

void UISceneManager::addWidgets() {

}

void UISceneManager::updateSelf(float delta) {
    LayerManager::update(delta);
}

std::shared_ptr<Scene> UISceneManager::createLayer(const std::string &name) {
    std::shared_ptr<Scene> scene = this->factory->getView(name);
    
    return scene;
}

void UISceneManager::addLayerToScene(std::shared_ptr<Scene>& node) {
    this->addChild(node);
    
    LayoutUtil::layoutParentCenter(node);

    return;
}

void UISceneManager::removeLayerFromScene(std::shared_ptr<Scene> &node) {
    node->removeFromParent();
}

std::shared_ptr<Scene> UISceneManager::getDefaultLayer() {
    return NULL;
}

void UISceneManager::onEnter() {
    LogicalScene::onEnter();
}

void UISceneManager::onExit() {
    LogicalScene::onExit();
}

void UISceneManager::keyBackClicked() {
    if (!isKeyBackActive) {
        return;
    }
    std::shared_ptr<Scene> scene = this->getLastLayer();
    
    if (scene) {
        scene->onBackKeyEvent();
    }
}

void UISceneManager::setKeyBackActive(bool active){
    isKeyBackActive = active;
}

    }
}
