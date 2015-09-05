#include "UIManager.h"
#include "Scene/Node.h"
#include "UI/Layout/LayoutUtil.h"
#include "Util/EventUtil.h"

namespace re {
namespace ui {

COMPONENT_DEPENDENCY(UIManager, Widget);

UIManager::UIManager()
    : isKeyBackActive(false)
{
}

void UIManager::onAwake() {
    this->getComponent<Transform2D>()->setSize(Screen::instance().getSize());

    isKeyBackActive = true;
}

SceneFactory &UIManager::getSceneFactory()
{
    return this->sceneFactory;
}

WindowFactory &UIManager::getWindowFactory()
{
    return this->windowFactory;
}

void UIManager::registerEvents()
{
    RegisterEvent(Events::Awake, this, &UIManager::onAwake);
    RegisterEvent(Events::Enter, this, &UIManager::onEnter);
    RegisterEvent(Events::Exit, this, &UIManager::onExit);
}

void UIManager::handleMessage(Message *message) {
    if (message->getType() == MessageConstant::MessageType::TOUCHSCREEN_MESSAGE) {
        TouchEvent* event = static_cast<TouchEvent*>(message->getData());

        event->setCurrPoint(event->getPoint() / Screen::instance().getFinalScale());
        this->getComponent<Widget>()->dispatchTouchEvent(*event);
    }
}

void UIManager::update() {
    LayerManager::tick();
}

ScenePtr UIManager::createLayer(const std::string &name) {
    ScenePtr scene = this->sceneFactory.createInstance(name);
    scene->getComponent<WindowManager>()->setWindowFactory(&windowFactory);
    
    return scene;
}

void UIManager::addLayerToScene(ScenePtr& node) {
    getNode()->addChild(node->getNode(), -2);
    
    LayoutUtil::LayoutToParent(node->getComponent<Transform2D>(), AlignType::CENTER, AlignType::CENTER);

    return;
}

void UIManager::removeLayerFromScene(ScenePtr &node) {
    node->getNode()->removeFromParent();
}

ScenePtr UIManager::getDefaultLayer() {
    return nullptr;
}

void UIManager::onEnter() {
    MessageManager::instance().addHandler(this);
}

void UIManager::onExit() {
    MessageManager::instance().removeHandler(this);
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

} // namespace ui
} // namespace re
