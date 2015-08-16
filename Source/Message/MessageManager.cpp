#include "MessageManager.h"

#include <algorithm>

const int MessageManager::DEFAULT_MESSAGE_ID = -1;

void MessageManager::sendSimpleMessage(int type, int id)
{
    Message *message = Message::obtainMessage();
    message->set(type, id);

    this->messageQueue.push_back(message);
}

void MessageManager::sendMessage(int type, int id, std::shared_ptr<void> obj)
{
    Message *message = Message::obtainMessage();
    message->set(type, id, obj);

    this->messageQueue.push_back(message);
}

void MessageManager::sendNoKeyMessage(int type, std::shared_ptr<void> obj)
{
    Message *message = Message::obtainMessage();
    message->set(type, DEFAULT_MESSAGE_ID, obj);

    this->messageQueue.push_back(message);
}

void MessageManager::addHandler(IMessageHandler *handler)
{
    auto iter = find(handlers.begin(), handlers.end(), handler);
    if (iter == handlers.end()) {
        this->handlers.push_back(handler);
    }
}

void MessageManager::removeHandler(IMessageHandler *handler)
{
    for (std::list<IMessageHandler *>::iterator iter = handlers.begin(); iter != handlers.end(); ++iter) {
        IMessageHandler *item = *iter;

        if (item == handler) {
            this->handlers.erase(iter);

            break;
        }
    }
}

bool MessageManager::checkHandled(Message* message) {
    if (message->IsHandled()) {
        if (find(messageHandled.begin(), messageHandled.end(), message) == messageHandled.end() ) {
            this->messageHandled.push_back(message);
        }
        
        return true;
    }
    
    return false;
}

void MessageManager::handleMessages()
{
    this->messageHandled.clear();

    for (auto message_iter = this->messageQueue.begin(); message_iter != this->messageQueue.end(); ++message_iter) {
        Message *message = *message_iter;

        if (!message || message->IsHandled()) {
            continue;
        }
        
        auto shadowHandles = handlers;
        
        for (auto iter = shadowHandles.begin(); iter != shadowHandles.end(); iter++) {
            IMessageHandler *handler = *iter;

            handler->handleMessage(message);

            if (this->checkHandled(message)) {
                // TODO: 场景应该吃掉所有消息
//                break;
            }
        }
        
        // TODO: 当前不考虑切换，消息不会驻留的
        if (!message->IsHandled()) {
            message->SetHandled();
        }
        
        checkHandled(message);
    }

    for (auto iter = this->messageHandled.begin(); iter != this->messageHandled.end(); ++iter) {
        Message *message = *iter;
        
        this->messageQueue.remove(message);
        
        if (*iter != NULL) {
            delete message;
        }
    }
}

