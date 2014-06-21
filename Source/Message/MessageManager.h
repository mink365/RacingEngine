#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "Message.h"

#include <list>
#include <memory>

using namespace std;

class IMessageHandler {
public:
    virtual void handleMessage(Message *message) = 0;
};

class MessageManager
{
public:
    static const int DEFAULT_MESSAGE_ID;

public:
    static MessageManager *getInstance();

    void sendSimpleMessage(int type, int id);
    void sendMessage(int type, int id, shared_ptr<void> obj);
    void sendNoKeyMessage(int type, shared_ptr<void> obj);

    void addHandler(IMessageHandler *handler);
    void removeHandler(IMessageHandler *handler);

    void handleMessages();

private:
    MessageManager();
    
    bool checkHandled(Message* msg);

private:
    std::list<Message *> messageQueue;
    std::list<Message *> messageHandled;

    std::list<IMessageHandler *> handlers;
};

#endif // MESSAGEMANAGER_H
