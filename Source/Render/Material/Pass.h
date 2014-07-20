#ifndef RE_PASS_H
#define RE_PASS_H

#include <memory>
#include <vector>

#include "TextureUnitState.h"

class Material;

namespace re {

class Pass : public Shared<Pass>,
             public Clonable<Pass>,
             public enable_shared_from_this<Pass>
{
    friend class Material;

public:
    Pass();

    void addTextureUnit(TextureUnitState::ptr& unit);
    TextureUnitState::ptr getTextureUnit(int index);
    int getTextureUnitCount() const;
    void clearTextureUnits();

    Pass::ptr clone() const override;
private:
    std::weak_ptr<Material> material;

    std::string name;
    int index;

    std::vector<TextureUnitState::ptr> units;
};

} // namespace re

#endif // RE_PASS_H
