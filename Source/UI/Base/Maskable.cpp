#include "Maskable.h"
#include "Scene/Node.h"
#include "Mask.h"

#include "Render/Material/Material.h"

namespace re {
namespace ui {

Maskable::Maskable()
{

}

void Maskable::UpdateInternalState()
{
    if (!shouldRefresh) {
        return;
    }

    int stencilValue = this->GetStencilValue();

    auto func = [](NodePtr& node) -> bool
    {
        auto mask = node->getComponent<Mask>();

        return mask == nullptr;
    };

    auto node = this->getNode();
    bool hasMask = DistpatchFunctionToTop(node, func);

    if (!hasMask) {
        return;
    }

    this->maskMaterial->getRenderState().stencilState.stencilTestEnable = true;
}

void Maskable::OnParentMaskStateChanged()
{
    this->shouldRefresh = true;
    this->UpdateInternalState();
}

int Maskable::GetStencilValue()
{
    return this->getNode()->getLevel();
}

} // namespace ui
} // namespace re

