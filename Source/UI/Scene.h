#ifndef __Scene__
#define __Scene__

#include <iostream>

#include "Manager/WindowManager.h"
#include "LogicalScene.h"
#include "AnimView.h"
#include "IBackKeyHandler.h"

namespace re {

class Scene : public LogicalScene, public WindowManager
{
public:
    Scene();
    virtual ~Scene();
    
    virtual void init();
        
    virtual bool onBackKeyEvent();
    
protected:
    virtual void updateSelf(float delta) override;
    
    virtual void onEnter();
    
protected:
    void addWindowToScene(std::shared_ptr<Window>& win);
    void removeWindowFromScene(std::shared_ptr<Window> &win);
    Node2d::ptr getAlphaBackground();
    void changeAlphaBackgroundIndex(std::shared_ptr<Window> &win);
    
private:
    Node2d::ptr alphaBg;
};

}

#endif /* defined(__Scene__) */
