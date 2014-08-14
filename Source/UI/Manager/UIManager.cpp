#include "UIManager.h"
#include "UI/Layout/LayoutUtil.h"

namespace re {
    namespace ui {

UIManager::UIManager()
: factory(nullptr)
, isKeyBackActive(false)
{
}

bool UIManager::init() {
    if (!LogicalScene::init()) {
        return false;
    }

    this->setContentSize(Screen::getInstance().getSize());

    isKeyBackActive = true;
    
    addWidgets();
    
    return true;
}

void UIManager::setSceneFactory(std::shared_ptr<ISceneFactory> &factory) {
    assert(this->factory == nullptr);
    
    this->factory = factory;
}

void UIManager::handleMessage(Message *message) {
//    LogicalScene::handleMessage(message);

    if (message->getType() == MessageConstant::MessageType::TOUCHSCREEN_MESSAGE) {
        TouchEvent* event = static_cast<TouchEvent*>(message->getData());

        event->setCurrPoint(event->getPoint() / Screen::getInstance().getFinalScale());
        this->dispatchTouchEvent(*event);
    }
}

void UIManager::addWidgets() {

}

void UIManager::updateSelf(float delta) {
    LayerManager::tick();
}

std::shared_ptr<Scene> UIManager::createLayer(const std::string &name) {
    std::shared_ptr<Scene> scene = this->factory->getView(name);
    
    return scene;
}

void UIManager::addLayerToScene(std::shared_ptr<Scene>& node) {
    this->addChild(node);
    
    LayoutUtil::layoutParentCenter(node);

    return;
}

void UIManager::removeLayerFromScene(std::shared_ptr<Scene> &node) {
    node->removeFromParent();
}

std::shared_ptr<Scene> UIManager::getDefaultLayer() {
    return NULL;
}

void UIManager::onEnter() {
    LogicalScene::onEnter();
}

void UIManager::onExit() {
    LogicalScene::onExit();
}

void UIManager::keyBackClicked() {
    if (!isKeyBackActive) {
        return;
    }
    std::shared_ptr<Scene> scene = this->getLastLayer();
    
    if (scene) {
        scene->onBackKeyEvent();
    }
}

void UIManager::setKeyBackActive(bool active){
    isKeyBackActive = active;
}

    }
}