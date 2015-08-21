#ifndef RE_ENTITY_H
#define RE_ENTITY_H

#include <vector>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <list>
#include "PreDeclare.h"
#include "Base/Clonable.h"
#include "Base/Named.h"

namespace re {

void DistpatchFunctionInHierarchy(NodePtr& root, std::function<void(NodePtr&)> func);
//void DistpatchFunctionInHierarchy(NodePtr& root, std::function<void(NodePtr&)>& func);

class Entity : public Named, public Clonable<Entity>, public enable_shared_from_this<Entity>
{
    friend class FbxParser;

public:
    Entity();

    void addComponent(ComponentPtr component);
//    template<typename T>
//    std::shared_ptr<T> addComponent<T>();

    void clearComponents();

    size_t getComponentCount() const;
    ComponentPtr getComponent(size_t index);
    const std::vector<ComponentPtr>& getComponents() const;

    template<typename T>
    std::shared_ptr<T> getComponent();

    template<typename T>
    std::shared_ptr<T> getComponentInParent();

    TransformPtr& getTransform();
    const TransformPtr& getTransform() const;
    void refreshTransformInHierarchy();

    NodePtr getNode();

    EntityPtr clone() const;

public:
    void Start();

private:
    void CacheComponents();

private:
    virtual void copyProperties(const Entity *entity);
    virtual void copyChildren(const Entity* entity);
    virtual void copyComponents(const Entity *entity);

private:
    virtual EntityPtr createCloneInstance() const;

private:
    std::vector<ComponentPtr> components;
    // same type component should never be add to one entity
    std::unordered_map<std::type_index, ComponentPtr> componentMap;

    struct CachedData {
        bool dirty = true;
        std::list<ComponentPtr> list;
    };
    std::unordered_map<std::type_index, CachedData> componentCacheMap;

    TransformPtr transform;
    NodePtr node;
};

inline NodePtr Entity::getNode()
{
    return this->node;
}

inline TransformPtr& Entity::getTransform()
{
    return transform;
}

inline const TransformPtr &Entity::getTransform() const
{
    return transform;
}

template<typename T>
inline std::shared_ptr<T> Entity::getComponent()
{
    auto id = std::type_index(typeid(T));

    if (componentMap.count(id) == 0) {
        auto& data = componentCacheMap[id];

        if (data.dirty) {
            for (auto& comp : components) {
                if (dynamic_cast<T*>(comp.get()))
                {
                    data.list.push_back(comp);
                }
            }

            data.dirty = false;
        }

        if (data.list.size() > 0) {
            return std::static_pointer_cast<T>(data.list.front());
        }
    } else {
        return std::static_pointer_cast<T>(componentMap[id]);
    }

    return nullptr;
}

} // namespace re

#endif // RE_ENTITY_H
