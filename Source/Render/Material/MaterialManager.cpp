#include "MaterialManager.h"

#include "Material.h"

namespace re {

MaterialManager::MaterialManager()
{
}

void MaterialManager::registerMaterial(MaterialPtr &m)
{
    this->materials[m->getName()] = m;
}

void MaterialManager::unRegisterMaterial(MaterialPtr &m)
{
    int count = this->materials.count(m->getName());
    if (count > 0) {
        auto iter = this->materials.find(m->getName());
        this->materials.erase(iter);
    }
}

MaterialPtr MaterialManager::getMaterial(const std::string name)
{
    auto iter = this->materials.find(name);

    if (iter != this->materials.end()) {
        return iter->second;
    }

    return nullptr;
}

} // namespace re
