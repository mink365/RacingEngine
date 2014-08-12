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

using namespace std;

template <class T>
class LayerManager {
public:
    virtual ~LayerManager();
    
    std::shared_ptr<T> pop();
    std::shared_ptr<T> popToRoot();
    std::shared_ptr<T> popTo(const std::string& name);
    
    /*
     * 清除原来的历史信息，直接跳转到新界面
     */
    std::shared_ptr<T> cleanTo(const std::string& name);
    std::shared_ptr<T> cleanTo(std::shared_ptr<T>& target);
    std::shared_ptr<T> pushTo(const std::string& name);
    std::shared_ptr<T> pushTo(std::shared_ptr<T>& target);
    
    void cleanAllLayer();
    
    std::shared_ptr<T> getLastLayer();
    int getStackSize();
    std::shared_ptr<T> getStackTop();
    
    std::shared_ptr<T> getLayerByName(const std::string& name);
    
    void tick();
    
private:
    void replaceCurLayer(std::shared_ptr<T>& old, std::shared_ptr<T>& target, bool immediately=false);
    
    void onLayerHidden(std::shared_ptr<T>& node);
    
protected:
    virtual void addLayerToScene(std::shared_ptr<T>& node) = 0;
    virtual void removeLayerFromScene(std::shared_ptr<T>& node) = 0;
    virtual std::shared_ptr<T> createLayer(const std::string& name) = 0;
    /*
     * get the default layer, if we have no layer in the stack, we will jump to it
     */
    virtual std::shared_ptr<T> getDefaultLayer() = 0;
    
    virtual void onItemPushToManager(std::shared_ptr<T>& item);
    virtual void onItemPopFromManager(std::shared_ptr<T>& item);

    virtual void onLayerStackPopToEmpty() {};
    
private:
    std::list<std::shared_ptr<T>> stack;
    std::list<std::shared_ptr<T>> oldLayerList;
};

template <class T>
inline LayerManager<T>::~LayerManager<T>() {
    while (!this->stack.empty()) {
        std::shared_ptr<T> item = stack.back();
        
        this->stack.pop_back();

        this->onItemPopFromManager(item);
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
        return this->stack.top();
    }
}

template <class T>
inline void LayerManager<T>::replaceCurLayer(std::shared_ptr<T> &old, std::shared_ptr<T> &target, bool immediately) {
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
    
    std::shared_ptr<T> old = this->stack.back();
        
    if (this->stack.size() <= 1) {
        std::shared_ptr<T> scene = this->getDefaultLayer();
        if (scene != NULL) {
            return this->cleanTo(scene);
        } else {
            this->onLayerStackPopToEmpty();
            return NULL;
        }
    } else {
        std::shared_ptr<T> item = stack.back();
        
        this->stack.pop_back();

        this->onItemPopFromManager(item);
    }
    
    std::shared_ptr<T> scene = this->stack.back();
    
    MessageManager::getInstance()->sendMessage(MessageConstant::MessageType::LAYER_MESSAGE,
                                               MessageConstant::LayerMessage::POP_LAYER,
                                               make_shared<string>(scene->getName()));
    
    this->replaceCurLayer(old, scene);
    
    return scene;
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::popTo(const std::string &name) {
    assert(!this->stack.empty());
    
    std::shared_ptr<T> top = this->stack.back();
    std::shared_ptr<T> old = top;
    
    while (!this->stack.empty()
           && top->getName() != name) {
        std::shared_ptr<T> item = stack.back();
        this->onItemPopFromManager(item);

        this->stack.pop_back();
        
        if (this->stack.empty()) {
            break;
        } else {
            top = this->stack.back();
        }
    }
    
    if (this->stack.empty()) {
        std::shared_ptr<T> scene = this->getDefaultLayer();
        if (scene != NULL) {
            this->stack.push_back(scene);
            this->onItemPushToManager(scene);
            
            this->replaceCurLayer(old, scene);
            
            return this->stack.back();
        } else {
            
        }
    } else {
        std::shared_ptr<T> scene = this->stack.back();
        
        this->replaceCurLayer(old, scene);
        
        return this->stack.back();
    }
    
    return NULL;
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::popToRoot() {
    assert(!this->stack.empty());
    
    std::shared_ptr<T> old = this->stack.back();
    
    if (this->stack.size() > 1) {
        while (this->stack.size() > 1) {
            std::shared_ptr<T> item = stack.back();
            
            this->stack.pop_back();
            
            this->onItemPopFromManager(item);
        }
    } else {
        std::shared_ptr<T> scene = this->getDefaultLayer();
        if (scene != NULL) {
            return this->cleanTo(scene);
        }
    }
    
    std::shared_ptr<T> scene = this->stack.back();
    
    this->replaceCurLayer(old, scene);
    
    return scene;
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::pushTo(const std::string& name) {
    std::shared_ptr<T> lastLayer = getLastLayer();
    if (lastLayer) {
        string lastLayerName = lastLayer->getName();
        if (lastLayerName == name) {
            return lastLayer;
        }
    }
    
    std::shared_ptr<T> scene = this->createLayer(name);
    
    return this->pushTo(scene);
    
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::pushTo(std::shared_ptr<T> &scene) {
    std::shared_ptr<T> old = NULL;
    
    if (stack.size() != 0) {
        old = stack.back();
    }
    
    if (old == scene) {
        return old;
    } else {
        this->stack.push_back(scene);
        this->onItemPushToManager(scene);
        
        MessageManager::getInstance()->sendMessage(MessageConstant::MessageType::LAYER_MESSAGE,
                                                   MessageConstant::LayerMessage::PUSH_LAYER,
                                                   make_shared<string>(scene->getName()));
        
        this->replaceCurLayer(old, scene);
        
        return scene;
    }
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::cleanTo(const std::string &name) {
    if (!stack.empty() && stack.size() == 1) {
        std::shared_ptr<T> old = stack.back();
        
        if (old->getName() == name) {
            return old;
        }
    }
    
    std::shared_ptr<T> scene = this->createLayer(name);
    
    return this->cleanTo(scene);
}

template <class T>
inline std::shared_ptr<T> LayerManager<T>::cleanTo(std::shared_ptr<T> &target) {
    std::shared_ptr<T> old = NULL;
    
    if (!stack.empty()) {
        old = stack.back();
    }
    
    std::shared_ptr<T> scene = target;
    
    if (old == scene && stack.size() == 1) {
        return old;
    }
    
    while (!this->stack.empty()) {
        std::shared_ptr<T> item = stack.back();
        
        this->stack.pop_back();
        
        this->onItemPopFromManager(item);
    }
    
    this->stack.push_back(scene);
    this->onItemPushToManager(scene);
    
    MessageManager::getInstance()->sendMessage(MessageConstant::MessageType::LAYER_MESSAGE,
                                               MessageConstant::LayerMessage::CLEAN_TO_LAYER,
                                               make_shared<string>(scene->getName()));
    this->replaceCurLayer(old, scene);
    
    return this->stack.back();
}

template <class T>
inline void LayerManager<T>::cleanAllLayer() {
    std::shared_ptr<T> old = NULL;
    
    if (!stack.empty()) {
        old = stack.back();
    }
    
    while (!this->stack.empty()) {
        std::shared_ptr<T> item = stack.back();
        
        this->stack.pop_back();
        
        this->onItemPopFromManager(item);
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
    auto query = find_if(begin(stack), end(stack), [name] (std::shared_ptr<T> layer) {
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
    auto iter = find_if(oldLayerList.begin(), oldLayerList.end(), [](std::shared_ptr<T> node) {
        return node->isHidden();
    });
    
    if (iter != oldLayerList.end()) {
        this->onLayerHidden(*iter);
        
        oldLayerList.erase(iter);
    }
}

template <class T>
inline void LayerManager<T>::onLayerHidden(std::shared_ptr<T> &node) {
    this->removeLayerFromScene(node);
}

template <class T>
inline void LayerManager<T>::onItemPushToManager(std::shared_ptr<T>& item) {
//    item->retain();
}

template <class T>
inline void LayerManager<T>::onItemPopFromManager(std::shared_ptr<T>& item) {
//    item->release();
}

}

#endif
