//
//  ViewFactory.h
//  FairyTails
//
//  Created by Rui Kou on 14-5-16.
//
//

#ifndef FairyTails_ViewFactory_h
#define FairyTails_ViewFactory_h

#include <string>

namespace re {

class Window;
class Scene;
class LogicalScene;

using namespace std;

class IWindowFactory {
public:
    virtual Window* getWindow(string name) = 0;
    
protected:
    virtual Window* createWindow(string name) = 0;
};


class ISceneFactory {
public:
    virtual Scene* getSceneByName(const string& name) = 0;
    
protected:
    virtual Scene* createScene(const string& name) = 0;
    virtual Scene* getRegisteredScene(const string& name) = 0;
    virtual void registerScene(const string& name, Scene* scene) = 0;
};

class ILayerFactory
{
public:
    virtual LogicalScene* getLayerByName(string layerName) = 0;
    
protected:
    virtual LogicalScene* createlayer(string  layerName) = 0;
    virtual LogicalScene* getRegisteredLayer(string  layerName) = 0;
    virtual void registerLayer(string name,LogicalScene* layer) = 0;
};

}

#endif
