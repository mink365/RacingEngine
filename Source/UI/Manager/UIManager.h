#ifndef __SceneManager__
#define __SceneManager__

#include <iostream>

#include "LayerManager.h"
#include "UI/Scene.h"
#include "ViewFactory.h"

namespace re {
    namespace ui {

class UIManager : public LogicalScene, public LayerManager<Scene> {
public:
    UIManager();
    bool init();
 
public:
    void setSceneFactory(std::shared_ptr<ISceneFactory>& factory);
    
public:
    void handleMessage(Message *message);
    void updateSelf(float delta);
    
    void addWidgets();
    
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackClicked();
    void setKeyBackActive(bool active);
protected:
    virtual std::shared_ptr<Scene> createLayer(const std::string& name);
    /*
     * get the default layer, if we have no layer in the stack, we will jump to it
     */
    virtual std::shared_ptr<Scene> getDefaultLayer();
    
    void addLayerToScene(std::shared_ptr<Scene> &node);
    void removeLayerFromScene(std::shared_ptr<Scene>& node);
    
private:
    bool isKeyBackActive;
    
    std::shared_ptr<ISceneFactory> factory;
};

    }
}

#endif /* defined(__SceneManager__) */
