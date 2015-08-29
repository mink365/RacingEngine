#ifndef RE_UI_MASK_H
#define RE_UI_MASK_H

#include "PreDeclare.h"
#include "Base/ECS/Component.h"

namespace re {
namespace ui {

class Mask : public Component
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
