#ifndef __LayerManager_h__
#define __LayerManager_h__

#include <stack>
#include <string>
#include <memory>
#include <algorithm>

#include "Message/MessageManager.h"
#include "Message/MessageConstant.h"
#include "UI/AnimView.h"
#include <assert.h>

namespace re {
namespace ui {

using namespace std;

template <class T>
class LayerManager {
public:
    typedef std::shared_ptr<T> ViewPtr;

public:
    virtual ~LayerManager();
    
    ViewPtr pop();
    ViewPtr popToRoot();
    ViewPtr popTo(const std::string& name);
    
    /*
     * 清除原来的历史信息，直接跳转到新界面
     */
    ViewPtr cleanTo(const std::string& name);
    ViewPtr cleanTo(ViewPtr& target);
    ViewPtr pushTo(const std::string& name);
    ViewPtr pushTo(ViewPtr& target);
    
    void cleanAllLayer();
    
    ViewPtr getLastLayer();
    int getStackSize();
    ViewPtr getStackTop();
    
    ViewPtr getLayerByName(const std::string& name);
    
    void tick();
    
private:
    void replaceCurLayer(ViewPtr& old, ViewPtr& target, bool immediately=false);
    
    void onLayerHidden(ViewPtr& node);
    
protected:
    virtual void addLayerToScene(ViewPtr& node) = 0;
    virtual void removeLayerFromScene(ViewPtr& node) = 0;
    virtual ViewPtr createLayer(const std::string& name) = 0;
    /*
     * get the default layer, if we have no layer in the stack, we will jump to it
     */
    virtual ViewPtr getDefaultLayer() = 0;

    virtual void onLayerStackPopToEmpty() {};
    
private:
    std::list<ViewPtr> stack;
    std::list<ViewPtr> oldLayerList;
};

template <class T>
inline LayerManager<T>::~LayerManager<T>() {
    while (!this->stack.empty()) {
        this->stack.pop_back();
    }
}

template <class T>
inline int LayerManager<T>::getStackSize() {
    return stack.size();
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::getStackTop() {
    if (this->stack.empty()) {
        return NULL;
    } else {
        return this->stack.back();
    }
}

template <class T>
inline void LayerManager<T>::replaceCurLayer(ViewPtr &old, ViewPtr &target, bool immediately) {
    this->addLayerToScene(target);
    
    target->show();
    
    if (old) {
        if (immediately) {
            old->hideImmed();
            this->removeLayerFromScene(old);
        } else {
            this->oldLayerList.push_back(old);
            old->hide();
        }
    }
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::pop() {
    assert(!this->stack.empty());
    
    ViewPtr old = this->stack.back();
        
    if (this->stack.size() <= 1) {
        ViewPtr scene = this->getDefaultLayer();
        if (scene != NULL) {
            return this->cleanTo(scene);
        } else {
            this->onLayerStackPopToEmpty();
            return NULL;
        }
    } else {
        this->stack.pop_back();
    }
    
    ViewPtr scene = this->stack.back();
    
    MessageManager::getInstance().sendMessage(MessageConstant::MessageType::LAYER_MESSAGE,
                                               MessageConstant::LayerMessage::POP_LAYER,
                                               make_shared<string>(scene->getName()));
    
    this->replaceCurLayer(old, scene);
    
    return scene;
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::popTo(const std::string &name) {
    assert(!this->stack.empty());
    
    ViewPtr top = this->stack.back();
    ViewPtr old = top;
    
    while (!this->stack.empty()
           && top->getName() != name) {
        this->stack.pop_back();
        
        if (this->stack.empty()) {
            break;
        } else {
            top = this->stack.back();
        }
    }
    
    if (this->stack.empty()) {
        ViewPtr scene = this->getDefaultLayer();
        if (scene != NULL) {
            this->stack.push_back(scene);
            
            this->replaceCurLayer(old, scene);
            
            return this->stack.back();
        } else {
            
        }
    } else {
        ViewPtr scene = this->stack.back();
        
        this->replaceCurLayer(old, scene);
        
        return this->stack.back();
    }
    
    return NULL;
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::popToRoot() {
    assert(!this->stack.empty());
    
    ViewPtr old = this->stack.back();
    
    if (this->stack.size() > 1) {
        while (this->stack.size() > 1) {
            this->stack.pop_back();
        }
    } else {
        ViewPtr scene = this->getDefaultLayer();
        if (scene != NULL) {
            return this->cleanTo(scene);
        }
    }
    
    ViewPtr scene = this->stack.back();
    
    this->replaceCurLayer(old, scene);
    
    return scene;
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::pushTo(const std::string& name) {
    ViewPtr lastLayer = getLastLayer();
    if (lastLayer) {
        string lastLayerName = lastLayer->getName();
        if (lastLayerName == name) {
            return lastLayer;
        }
    }
    
    ViewPtr scene = this->createLayer(name);
    
    return this->pushTo(scene);
    
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::pushTo(ViewPtr &scene) {
    ViewPtr old = NULL;
    
    if (stack.size() != 0) {
        old = stack.back();
    }
    
    if (old == scene) {
        return old;
    } else {
        this->stack.push_back(scene);
        
        MessageManager::getInstance().sendMessage(MessageConstant::MessageType::LAYER_MESSAGE,
                                                   MessageConstant::LayerMessage::PUSH_LAYER,
                                                   make_shared<string>(scene->getName()));
        
        this->replaceCurLayer(old, scene);
        
        return scene;
    }
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::cleanTo(const std::string &name) {
    if (!stack.empty() && stack.size() == 1) {
        ViewPtr old = stack.back();
        
        if (old->getName() == name) {
            return old;
        }
    }
    
    ViewPtr scene = this->createLayer(name);
    
    return this->cleanTo(scene);
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::cleanTo(ViewPtr &target) {
    ViewPtr old = NULL;
    
    if (!stack.empty()) {
        old = stack.back();
    }
    
    ViewPtr scene = target;
    
    if (old == scene && stack.size() == 1) {
        return old;
    }
    
    while (!this->stack.empty()) {
        this->stack.pop_back();
    }
    
    this->stack.push_back(scene);
    
    MessageManager::getInstance().sendMessage(MessageConstant::MessageType::LAYER_MESSAGE,
                                               MessageConstant::LayerMessage::CLEAN_TO_LAYER,
                                               make_shared<string>(scene->getName()));
    this->replaceCurLayer(old, scene);
    
    return this->stack.back();
}

template <class T>
inline void LayerManager<T>::cleanAllLayer() {
    ViewPtr old = NULL;
    
    if (!stack.empty()) {
        old = stack.back();
    }
    
    while (!this->stack.empty()) {
        this->stack.pop_back();
    }
    
    if (old != NULL) {
        old->hideImmed();
        this->removeLayerFromScene(old);
    }
    
    for (auto layer : oldLayerList) {
        layer->hideImmed();
        this->removeLayerFromScene(layer);
    }
    oldLayerList.clear();
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::getLastLayer() {
    if (this->stack.size() >= 1) {
        // TODO:
        return this->stack.back();
    } else {
        return NULL;
    }
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::getLayerByName(const std::string& name) {
    auto query = find_if(begin(stack), end(stack), [name] (ViewPtr layer) {
        return layer->getName() == name;
    });
    if (query != end(stack)) {
        return *query;
    } else {
        return NULL;
    }
}

template <class T>
inline void LayerManager<T>::tick() {
    auto iter = find_if(oldLayerList.begin(), oldLayerList.end(), [](ViewPtr node) {
        return node->isHidden();
    });
    
    if (iter != oldLayerList.end()) {
        this->onLayerHidden(*iter);
        
        oldLayerList.erase(iter);
    }
}

template <class T>
inline void LayerManager<T>::onLayerHidden(ViewPtr &node) {
    this->removeLayerFromScene(node);
}

} // namespace ui
} // namespace re

#endif
