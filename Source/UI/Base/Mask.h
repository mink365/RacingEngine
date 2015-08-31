#ifndef RE_UI_MASK_H
#define RE_UI_MASK_H

#include "Base/ECS/Component.h"

namespace re {

class Material;
using MaterialPtr = SharedPtr<Material>;

namespace ui {

class Mask : public Component<Mask>
{
public:
    Mask();

public:
    void UpdateInternalState();

private:
    MaterialPtr maskMaterial;

    bool shouldRefresh = true;
};

} // namespace ui
} // namespace re

#endif // RE_UI_MASK_H
