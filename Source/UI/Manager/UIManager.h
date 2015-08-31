#ifndef __SceneManager__
#define __SceneManager__

#include <iostream>

#include "LayerManager.h"
#include "UI/Scene.h"
#include "Base/NamedFactory.h"

namespace re {
namespace ui {

typedef NamedComponentFactory<Window> WindowFactory;
typedef NamedComponentFactory<Scene> SceneFactory;
typedef ComponentHandle<Scene> ScenePtr;

class UIManager : public Component<UIManager>, public LayerManager<Scene>, public IMessageHandler {
public:
    UIManager();
 
public:
    SceneFactory& getSceneFactory();
    WindowFactory& getWindowFactory();
    
public:
    void registerEvents();

public:
    void handleMessage(Message *message);
    void update();
    
    void onAwake();
    void onEnter();
    void onExit();
    
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

} // namespace ui
} // namespace re

#endif /* defined(__SceneManager__) */
