#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Base/Singleton.h"
#include <memory>
#include <unordered_map>

namespace re {

template<class T>
class ResourceManager : public Singleton<ResourceManager<T>>
{
public:
    void Register(std::shared_ptr<T> r);
    void Dispose(std::shared_ptr<T> r);

    bool Contain(const std::string& name);
    std::shared_ptr<T> GetResource(const std::string& name);

private:
    std::unordered_map<std::string, std::shared_ptr<T>> map;
};

template<class T>
inline void ResourceManager<T>::Register(std::shared_ptr<T> r)
{
    map[r->getName()] = r;
}

template<class T>
inline void ResourceManager<T>::Dispose(std::shared_ptr<T> r)
{
    auto iter = map.find(r->getName());

    if (iter != map.end()) {
        map.erase(iter);
    }
}

template<class T>
inline bool ResourceManager<T>::Contain(const std::string &name)
{
    return (map.find(name) != map.end());
}

template<class T>
inline std::shared_ptr<T> ResourceManager<T>::GetResource(const std::string &name)
{
    return map[name];
}

}

#endif // RESOURCEMANAGER_H
