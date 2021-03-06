#ifndef RE_UI_MASKABLE_H
#define RE_UI_MASKABLE_H

#include "Base/ECS/Component.h"

namespace re {

class Material;
using MaterialPtr = SharedPtr<Material>;

namespace ui {

class Maskable : public Component<Maskable>
{
public:
    Maskable();

public:
    void UpdateInternalState();
    void OnParentMaskStateChanged();

private:
    int GetStencilValue();

private:
    MaterialPtr maskMaterial;
    int stencilValue = 0;

    bool shouldRefresh = true;
};

} // namespace ui
} // namespace re

#endif // RE_UI_MASKABLE_H
