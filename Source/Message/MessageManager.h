#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "Message.h"
#include "Base/Singleton.h"

#include <list>
#include <memory>

class IMessageHandler {
public:
    virtual void handleMessage(Message *message) = 0;
};

class MessageManager : public re::Singleton<MessageManager>
{
public:
    static const int DEFAULT_MESSAGE_ID;

public:
    void sendSimpleMessage(int type, int id);
    void sendMessage(int type, int id, std::shared_ptr<void> obj=nullptr);
    void sendNoKeyMessage(int type, std::shared_ptr<void> obj=nullptr);

    void addHandler(IMessageHandler *handler);
    void removeHandler(IMessageHandler *handler);

    void handleMessages();

private:
    bool checkHandled(Message* msg);

private:
    std::list<Message *> messageQueue;
    std::list<Message *> messageHandled;

    std::list<IMessageHandler *> handlers;
};

#endif // MESSAGEMANAGER_H
