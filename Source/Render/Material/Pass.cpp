#include "Pass.h"

namespace re {

Pass::Pass()
{
}

Pass::ptr Pass::clone() const
{
    Pass::ptr inst = Pass::create();

    inst->material.reset();
    inst->name = this->name;
    inst->index = 0;

    return inst;
}

} // namespace re
