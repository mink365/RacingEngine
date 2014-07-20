#ifndef RE_MATERIALMANAGER_H
#define RE_MATERIALMANAGER_H

#include "PredefineType.h"
#include "Base/Singleton.h"
#include <map>

namespace re {

class MaterialManager : public Singleton<MaterialManager>
{
public:
    MaterialManager();

    void registerMaterial(MaterialPtr& m);
    void unRegisterMaterial(MaterialPtr& m);

    MaterialPtr getMaterial(const std::string name);

private:
    std::map<std::string, MaterialPtr> materials;
};

} // namespace re

#endif // RE_MATERIALMANAGER_H
