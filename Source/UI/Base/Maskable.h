#ifndef RE_UI_MASKABLE_H
#define RE_UI_MASKABLE_H

#include "PreDeclare.h"
#include "Component.h"

namespace re {
namespace ui {

class Maskable : public Component
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
