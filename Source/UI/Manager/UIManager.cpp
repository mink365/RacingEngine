#include "UIManager.h"
#include "UI/Layout/LayoutUtil.h"

namespace re {
    namespace ui {

UIManager::UIManager()
    : isKeyBackActive(false)
{
}

bool UIManager::init() {
    this->setContentSize(Screen::getInstance().getSize());

    isKeyBackActive = true;
    
    addWidgets();
    
    return true;
}

SceneFactory &UIManager::getSceneFactory()
{
    return this->sceneFactory;
}

WindowFactory &UIManager::getWindowFactory()
{
    return this->windowFactory;
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
    std::shared_ptr<Scene> scene = this->sceneFactory.createInstance(name);
    scene->setWindowFactory(&windowFactory);
    
    return scene;
}

void UIManager::addLayerToScene(std::shared_ptr<Scene>& node) {
    this->addChild(node);
    
    LayoutUtil::LayoutToParentCenter(node);

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
