//
//  SceneManager.h
//  MT
//
//  Created by 寇 瑞 on 7/23/13.
//
//

#ifndef __MT__SceneManager__
#define __MT__SceneManager__

#include <iostream>

#include "LayerManager.h"
#include "UI/Scene.h"
#include "ViewFactory.h"

namespace re {
    namespace ui {

class UISceneManager : public LogicalScene, public LayerManager<Scene> {
private:
    UISceneManager();
    bool init();
 
public:
//    GuideLayer* getGuidLayer();
    
    void setSceneFactory(ISceneFactory* factory);
    
public:
    void handleMessage(Message *message);
    void updateSelf(float delta);
    
    void addWidgets();
    
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackClicked();
    void setKeyBackActive(bool active);
protected:
    virtual Scene* createLayer(const std::string& name);
    /*
     * get the default layer, if we have no layer in the stack, we will jump to it
     */
    virtual Scene* getDefaultLayer();
    
    void addLayerToScene(Scene* node);
    void removeLayerFromScene(Scene* node);
    
private:
    bool isKeyBackActive;
    
    ISceneFactory* factory;
};

    }
}

#endif /* defined(__MT__SceneManager__) */
