#include "Maskable.h"
#include "Scene/Node.h"
#include "Mask.h"
#include "Graphic.h"

#include "Render/Material/Material.h"

namespace re {
namespace ui {

COMPONENT_DEPENDENCY(Maskable, CanvasRenderElement);

Maskable::Maskable()
{

}

void Maskable::UpdateInternalState()
{
    if (!shouldRefresh) {
        return;
    }
    this->shouldRefresh = false;

//    int stencilValue = this->GetStencilValue();

    ComponentHandle<Mask> mask = nullptr;
    auto func = [&](NodePtr& node) -> bool
    {
        auto _mask = node->getComponent<Mask>();

        if (_mask != nullptr) {
            mask = _mask;
        }

        return _mask == nullptr;
    };

    auto node = this->getNode();
    bool hasMask = DistpatchFunctionToTop(node, func);

    if (!hasMask) {
        return;
    }

    int stencilValue = mask->getNode()->getLevel();

    auto element = this->getComponent<CanvasRenderElement>();
    this->maskMaterial = element->getMaterial();
    StencilState& stencilState = this->maskMaterial->getRenderState().stencilState;
    stencilState.testEnable = true;
    stencilState.function = TestFunction::Equal;
    stencilState.refValue = stencilValue;
    stencilState.maskValue = 0x00;
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

