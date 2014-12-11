#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <memory>

using namespace std;

class Message
{
    friend class MessageManager;

public:
    void set(int type, int id);
    void set(int type, int id, std::shared_ptr<void> data);
    void set(const Message& message);

    void setSourceTag(std::string tag);

    static Message *obtainMessage();
    static void releaseMessage(Message* m);

    bool IsHandled() const;
    void SetHandled();

    int getType();
    int getId();

    void *getData();

protected:
    Message();

private:
    int type;
    int id;

    long eventTime;
    bool isHandled;

    std::shared_ptr<void> data;
};

#endif // MESSAGE_H
