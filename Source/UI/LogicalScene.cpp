#include "LogicalScene.h"

namespace re {

LogicalScene::LogicalScene(){
    setName("");
}

LogicalScene::LogicalScene(string name){
    setName(name);
}

void LogicalScene::onEnter(){
    MessageManager::getInstance().addHandler(this);
}

void LogicalScene::onExit(){
    MessageManager::getInstance().removeHandler(this);
}

void LogicalScene::handleMessage(Message *message){
    
}

bool LogicalScene::onBackKeyEvent() {
    return false;
}

}
