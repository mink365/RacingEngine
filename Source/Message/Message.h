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

    const void* getData() const;

protected:
    Message();

private:
    int type;
    int id;

    /**
     * @brief source
     * 消息来源的标识。比如，我们在某个界面点了按钮，发出了某个RPC请求，
     * 但我们可能有两个地方在监测此事件，这就很难确定如何处理。使用一个
     * 标识来辨别它
     */
    std::string source;

    long eventTime;
    bool isHandled;

    std::shared_ptr<void> data;
};

#endif // MESSAGE_H
