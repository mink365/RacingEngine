#ifndef RE_MATERIALLOADER_H
#define RE_MATERIALLOADER_H

#include "Base/Singleton.h"
#include "PredefineType.h"
#include "Material/Material.h"
#include "Resource/Common/BlockParser.h"

namespace re {

template<class T>
T To(const std::string& v);
template<>
int To<int>(const std::string& v) {
    return std::stoi(v);
}
template<>
bool To<bool>(const std::string& v) {
    if (v == "true") {
        return true;
    } else {
        return false;
    }
}

class MaterialLoader : public Singleton<MaterialLoader>
{
public:
    MaterialLoader();

    MaterialPtr Load(FilePtr& file);

private:
    void loadMaterial(Statement::ptr& statement);
    void loadPass(Statement::ptr& statement, Pass::ptr& pass);
    void loadTextureUnit(Statement::ptr& statement, TextureUnitState::ptr& unit);
};

} // namespace re

#endif // RE_MATERIALLOADER_H
