//
//  Scene.h
//  MT
//
//  Created by 寇 瑞 on 7/10/13.
//
//

#ifndef __MT__Scene__
#define __MT__Scene__

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
    
    virtual bool init();
        
//    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    
    virtual bool onBackKeyEvent();
    
protected:
    void update(float dt);
    
    virtual void onEnter();
    
protected:
    void addWindowToScene(Window* win);
    void removeWindowFromScene(Window* win);
    Node2d::ptr getAlphaBackground();
    void changeAlphaBackgroundIndex(Window* win);
    
private:
    Node2d::ptr alphaBg;
};

}

#endif /* defined(__MT__Scene__) */
