#include "UIManager.h"
#include "UI/Layout/LayoutUtil.h"

namespace re {
    namespace ui {

UIManager::UIManager()
    : isKeyBackActive(false)
{
}

void UIManager::init() {
    this->getComponent<Transform2D>()->setContentSize(Screen::getInstance().getSize());

    isKeyBackActive = true;
    
    addWidgets();
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

ScenePtr UIManager::createLayer(const std::string &name) {
    ScenePtr scene = this->sceneFactory.createInstance(name);
    scene->setWindowFactory(&windowFactory);
    
    return scene;
}

void UIManager::addLayerToScene(ScenePtr& node) {
    getNode()->addChild(node->getNode(), -2);
    
    LayoutUtil::LayoutToParentCenter(node->getComponent<Transform2D>());

    return;
}

void UIManager::removeLayerFromScene(ScenePtr &node) {
    node->getNode()->removeFromParent();
}

ScenePtr UIManager::getDefaultLayer() {
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
    ScenePtr scene = this->getLastLayer();
    
    if (scene) {
        scene->onBackKeyEvent();
    }
}

void UIManager::setKeyBackActive(bool active){
    isKeyBackActive = active;
}

    }
}
