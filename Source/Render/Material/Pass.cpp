#include "Pass.h"

namespace re {

Pass::Pass()
{
}

void Pass::addTextureUnit(TextureUnitState::ptr &unit)
{
    this->units.push_back(unit);
    unit->pass = this->shared_from_this();
}

TextureUnitState::ptr Pass::getTextureUnit(int index)
{
    return this->units.at(index);
}

int Pass::getTextureUnitCount() const
{
    return this->units.size();
}

void Pass::clearTextureUnits()
{
    this->units.clear();
}

Pass::ptr Pass::clone() const
{
    Pass::ptr inst = Pass::create();

    inst->material.reset();
    inst->name = this->name;
    inst->index = 0;
    for (auto unit : this->units) {
        TextureUnitState::ptr unitInst = unit->clone();

        inst->addTextureUnit(unitInst);
    }

    return inst;
}

} // namespace re
