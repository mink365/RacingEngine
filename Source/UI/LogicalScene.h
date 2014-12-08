#ifndef LOGICALSCENE_H
#define LOGICALSCENE_H

#include "Message/MessageManager.h"
#include "string"
#include "Widget.h"
#include "AnimView.h"
#include "IBackKeyHandler.h"

namespace re {

using namespace std;

class LogicalScene : public Widget, public AnimationView, public IMessageHandler, public IBackKeyHandler
{
public:
    LogicalScene();
    LogicalScene(string name);
    
    virtual ~LogicalScene() {};

    virtual void onEnter();
    virtual void onExit();

    virtual void handleMessage(Message *message);
    
public:
    virtual bool onBackKeyEvent();
};

}

#endif // LOGICALSCENE_H
