#ifndef __SceneManager__
#define __SceneManager__

#include <iostream>

#include "LayerManager.h"
#include "UI/Scene.h"
#include "Base/NamedFactory.h"

namespace re {
typedef NamedFactory<Window> WindowFactory;
typedef NamedFactory<Scene> SceneFactory;
typedef std::shared_ptr<Scene> ScenePtr;

    namespace ui {

class UIManager : public Widget, public LayerManager<Scene>, public IMessageHandler {
public:
    UIManager();
    void init();
 
public:
    SceneFactory& getSceneFactory();
    WindowFactory& getWindowFactory();
    
public:
    void handleMessage(Message *message);
    void update();
    
    void addWidgets();
    
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackClicked();
    void setKeyBackActive(bool active);
protected:
    virtual ScenePtr createLayer(const std::string& name);
    /*
     * get the default layer, if we have no layer in the stack, we will jump to it
     */
    virtual ScenePtr getDefaultLayer();
    
    void addLayerToScene(ScenePtr &node);
    void removeLayerFromScene(ScenePtr& node);
    
private:
    bool isKeyBackActive;

    SceneFactory sceneFactory;
    WindowFactory windowFactory;
};

    }
}

#endif /* defined(__SceneManager__) */
