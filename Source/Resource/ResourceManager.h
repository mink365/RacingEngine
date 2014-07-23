#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Base/Singleton.h"
#include <memory>
#include <map>

#include "Texture/Texture.h"

namespace re {

template<class T, class V>
class ResouceManager : public Singleton<V>
{
public:
    void Register(std::shared_ptr<T> r);
    void Dispose(std::shared_ptr<T> r);

    bool Contain(const std::string& name);
    std::shared_ptr<T> GetResource(const std::string& name);

private:
    std::map<std::string, std::shared_ptr<T>> map;
};

template<class T, class V>
inline void ResouceManager<T, V>::Register(std::shared_ptr<T> r)
{
    map[r->getName()] = r;
}

template<class T, class V>
inline void ResouceManager<T, V>::Dispose(std::shared_ptr<T> r)
{
    auto iter = map.find(r->getName());

    if (iter != map.end()) {
        map.erase(iter);
    }
}

template<class T, class V>
inline bool ResouceManager<T, V>::Contain(const string &name)
{
    return (map.find(name) != map.end());
}

template<class T, class V>
inline std::shared_ptr<T> ResouceManager<T, V>::GetResource(const string &name)
{
    return map[name];
}

class TexManager : public ResouceManager<Texture, TexManager>
{
public:
    TexManager() {
    }

    void Hello() {
        int i = 0;
    }
};

}

#endif // RESOURCEMANAGER_H
