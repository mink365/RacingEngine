//
//  LayerManager.h
//  MT
//
//  Created by 寇 瑞 on 4/26/13.
//
//

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
    
    T* pop();
    T* popToRoot();
    T* popTo(const std::string& name);
    
    /*
     * 清除原来的历史信息，直接跳转到新界面
     */
    T* cleanTo(const std::string& name);
    T* cleanTo(T* target);
    T* pushTo(const std::string& name);
    T* pushTo(T* target);
    
    void cleanAllLayer();
    
    T* getLastLayer();
    int getStackSize();
    T* getStackTop();
    
    T* getLayerByName(const std::string& name);
    
    void update(float dt);
    
private:
    void replaceCurLayer(T* old, T* target, bool immediately=false);
    
    void onLayerHidden(T* node);
    
protected:
    virtual void addLayerToScene(T* node) = 0;
    virtual void removeLayerFromScene(T* node) = 0;
    virtual T* createLayer(const std::string& name) = 0;
    /*
     * get the default layer, if we have no layer in the stack, we will jump to it
     */
    virtual T* getDefaultLayer() = 0;
    
    virtual void onItemPushToManager(T* item);
    virtual void onItemPopFromManager(T* item);

    virtual void onLayerStackPopToEmpty() {};
    
private:
    std::list<T*> stack;
    std::list<T*> oldLayerList;
};

template <class T>
inline LayerManager<T>::~LayerManager<T>() {
    while (!this->stack.empty()) {
        T* item = stack.back();
        
        this->stack.pop_back();

        this->onItemPopFromManager(item);
    }
}

template <class T>
inline int LayerManager<T>::getStackSize() {
    return stack.size();
}

template <class T>
inline T* LayerManager<T>::getStackTop() {
    if (this->stack.empty()) {
        return NULL;
    } else {
        return this->stack.top();
    }
}

template <class T>
inline void LayerManager<T>::replaceCurLayer(T *old, T *target, bool immediately) {
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
inline T* LayerManager<T>::pop() {
    assert(!this->stack.empty());
    
    T* old = this->stack.back();
        
    if (this->stack.size() <= 1) {
        T* scene = this->getDefaultLayer();
        if (scene != NULL) {
            return this->cleanTo(scene);
        } else {
            this->onLayerStackPopToEmpty();
            return NULL;
        }
    } else {
        T* item = stack.back();
        
        this->stack.pop_back();

        this->onItemPopFromManager(item);
    }
    
    T* scene = this->stack.back();
    
    MessageManager::getInstance()->sendMessage(MessageConstant::MessageType::LAYER_MESSAGE,
                                               MessageConstant::LayerMessage::POP_LAYER,
                                               make_shared<string>(scene->getName()));
    
    this->replaceCurLayer(old, scene);
    
    return scene;
}

template <class T>
inline T* LayerManager<T>::popTo(const std::string &name) {
    assert(!this->stack.empty());
    
    T* top = this->stack.back();
    T* old = top;
    
    while (!this->stack.empty()
           && top->getName() != name) {
        T* item = stack.back();
        this->onItemPopFromManager(item);

        this->stack.pop_back();
        
        if (this->stack.empty()) {
            break;
        } else {
            top = this->stack.back();
        }
    }
    
    if (this->stack.empty()) {
        T* scene = this->getDefaultLayer();
        if (scene != NULL) {
            this->stack.push_back(scene);
            this->onItemPushToManager(scene);
            
            this->replaceCurLayer(old, scene);
            
            return this->stack.back();
        } else {
            
        }
    } else {
        T* scene = this->stack.back();
        
        this->replaceCurLayer(old, scene);
        
        return this->stack.back();
    }
    
    return NULL;
}

template <class T>
inline T* LayerManager<T>::popToRoot() {
    assert(!this->stack.empty());
    
    T* old = this->stack.back();
    
    if (this->stack.size() > 1) {
        while (this->stack.size() > 1) {
            T* item = stack.back();
            
            this->stack.pop_back();
            
            this->onItemPopFromManager(item);
        }
    } else {
        T* scene = this->getDefaultLayer();
        if (scene != NULL) {
            return this->cleanTo(scene);
        }
    }
    
    T* scene = this->stack.back();
    
    this->replaceCurLayer(old, scene);
    
    return scene;
}

template <class T>
inline T* LayerManager<T>::pushTo(const std::string& name) {
    T* lastLayer = getLastLayer();
    if (lastLayer) {
        string lastLayerName = lastLayer->getName();
        if (lastLayerName == name) {
            return lastLayer;
        }
    }
    
    T* scene = this->createLayer(name);
    
    return this->pushTo(scene);
    
}

template <class T>
inline T* LayerManager<T>::pushTo(T* scene) {
    T* old = NULL;
    
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
inline T* LayerManager<T>::cleanTo(const std::string &name) {
    if (!stack.empty() && stack.size() == 1) {
        T* old = stack.back();
        
        if (old->getName() == name) {
            return old;
        }
    }
    
    T* scene = this->createLayer(name);
    
    return this->cleanTo(scene);
}

template <class T>
inline T* LayerManager<T>::cleanTo(T* target) {
    T* old = NULL;
    
    if (!stack.empty()) {
        old = stack.back();
    }
    
    T* scene = target;
    
    if (old == scene && stack.size() == 1) {
        return old;
    }
    
    while (!this->stack.empty()) {
        T* item = stack.back();
        
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
    T* old = NULL;
    
    if (!stack.empty()) {
        old = stack.back();
    }
    
    while (!this->stack.empty()) {
        T* item = stack.back();
        
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
inline T* LayerManager<T>::getLastLayer() {
    if (this->stack.size() >= 1) {
        // TODO:
        return this->stack.back();
    } else {
        return NULL;
    }
}

template <class T>
inline T* LayerManager<T>::getLayerByName(const std::string& name) {
    auto query = find_if(begin(stack), end(stack), [name] (T* layer) {
        return layer->getName() == name;
    });
    if (query != end(stack)) {
        return *query;
    } else {
        return NULL;
    }
}

template <class T>
inline void LayerManager<T>::update(float dt) {
    auto iter = find_if(oldLayerList.begin(), oldLayerList.end(), [](T* node) {
        return node->isHidden();
    });
    
    if (iter != oldLayerList.end()) {
        this->onLayerHidden(*iter);
        
        oldLayerList.erase(iter);
    }
}

template <class T>
inline void LayerManager<T>::onLayerHidden(T *node) {
    this->removeLayerFromScene(node);
}

template <class T>
inline void LayerManager<T>::onItemPushToManager(T* item) {
//    item->retain();
}

template <class T>
inline void LayerManager<T>::onItemPopFromManager(T* item) {
//    item->release();
}

}

#endif
