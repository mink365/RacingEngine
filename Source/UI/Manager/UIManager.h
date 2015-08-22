#ifndef __SceneManager__
#define __SceneManager__

#include <iostream>

#include "LayerManager.h"
#include "UI/Scene.h"
#include "Base/NamedFactory.h"

namespace re {
namespace ui {

typedef NamedFactory<Window> WindowFactory;
typedef NamedFactory<Scene> SceneFactory;
typedef SharedPtr<Scene> ScenePtr;

class UIManager : public Widget, public LayerManager<Scene>, public IMessageHandler {
public:
    UIManager();
    void init();
 
public:
    SceneFactory& getSceneFactory();
    WindowFactory& getWindowFactory();
    
public:
    void RegisterEvents();

public:
    void handleMessage(Message *message);
    void update();
    
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
