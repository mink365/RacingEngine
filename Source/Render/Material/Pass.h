#ifndef RE_PASS_H
#define RE_PASS_H

#include <memory>
#include <vector>

#include "SamplerParameter.h"

namespace re {

class Material;

class Pass : public Shared<Pass>,
             public Clonable<Pass>
{
    friend class Material;

public:
    Pass();

    Pass::ptr clone() const override;
private:
    std::weak_ptr<Material> material;

    std::string name;
    int index;
};

} // namespace re

#endif // RE_PASS_H
