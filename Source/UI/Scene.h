#ifndef __Scene__
#define __Scene__

#include <iostream>

#include "Manager/WindowManager.h"
#include "AnimView.h"
#include "IBackKeyHandler.h"

namespace re {
namespace ui {

class Scene :  public Widget, public AnimationView, public IMessageHandler, public IBackKeyHandler
{
public:
    Scene();
    virtual ~Scene();
    
    virtual void init();

    virtual void handleMessage(Message *message);

    virtual bool onBackKeyEvent();
    
protected:
    virtual void onEnter();
    virtual void onExit();
};

} // namespace ui
} // namespace re

#endif /* defined(__Scene__) */
