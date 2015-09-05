#include "Mask.h"
#include "Scene/Node.h"
#include "Graphic.h"

#include "Render/Material/Material.h"

namespace re {
namespace ui {

COMPONENT_DEPENDENCY(Mask, CanvasRenderElement);

Mask::Mask()
{

}

void Mask::UpdateInternalState()
{
    if (!shouldRefresh) {
        return;
    }
    this->shouldRefresh = false;

    int stencilValue = this->getNode()->getLevel();

    auto element = this->getComponent<CanvasRenderElement>();
    this->maskMaterial = element->getMaterial();
    StencilState& stencilState = this->maskMaterial->getRenderState().stencilState;
    stencilState.testEnable = true;
    stencilState.function = TestFunction::Always;
    stencilState.depthPassOperation = StencilOperation::Replace;
    stencilState.refValue = stencilValue;
    stencilState.maskValue = 0xFF;
}

} // namespace ui
} // namespace re

