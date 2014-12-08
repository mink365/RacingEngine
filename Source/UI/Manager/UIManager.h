#ifndef __SceneManager__
#define __SceneManager__

#include <iostream>

#include "LayerManager.h"
#include "UI/Scene.h"
#include "Base/NamedFactory.h"

namespace re {
typedef NamedFactory<Window> WindowFactory;
typedef NamedFactory<Scene> SceneFactory;

    namespace ui {

class UIManager : public LogicalScene, public LayerManager<Scene> {
public:
    UIManager();
    bool init();
 
public:
    SceneFactory& getSceneFactory();
    WindowFactory& getWindowFactory();
    
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

    SceneFactory sceneFactory;
    WindowFactory windowFactory;
};

    }
}

#endif /* defined(__SceneManager__) */
