#include "Message.h"
#include "Util/LocalTime.h"

Message::Message()
: type(-1)
, id(-1)
, data(NULL)
, isHandled(false)
, eventTime(-1)
, source("")
{
}

void Message::set(int type, int id)
{
    this->set(type, id, nullptr);
}

void Message::set(int type, int id, std::shared_ptr<void> data)
{
    this->isHandled = false;
    this->eventTime = LocalTime::getInstance().getCurrentTime();

    this->type = type;
    this->id = id;
    
    this->data = data;
}

void Message::set(const Message& message) {
    type = message.type;
    id = message.id;
    
    source = message.source;
    
    eventTime = message.eventTime;
    isHandled = message.isHandled;
    
    data = message.data;
}

Message *Message::obtainMessage()
{
    return new Message();
}

void Message::releaseMessage(Message *m)
{
    delete m;
}

bool Message::IsHandled() const
{
    return isHandled;
}

void Message::SetHandled()
{
    isHandled = true;
}

int Message::getType()
{
    return this->type;
}

int Message::getId()
{
    return this->id;
}

const void *Message::getData() const
{
    return this->data.get();
}
