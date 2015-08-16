#ifndef RE_UI_ELEMENTBATCHER_H
#define RE_UI_ELEMENTBATCHER_H

#include <unordered_map>
#include <set>
#include "PreDeclare.h"
#include "Component.h"
#include "ElementBatch.h"
#include "CanvasRenderElement.h"

namespace re {
namespace ui {

class ElementBatcher : public Component
{
public:
    ElementBatcher();

    /**
     * @brief ReBuild
     * step the hierarchy and batch all the RenderElement
     */
    void ReBuild();

    /**
     * @brief Render
     * append all ElementBatch To the Renderer
     */
    void Render();

private:
    ElementBatch::ptr FindBatchForElement(const CanvasRenderElement& element);

private:
    std::unordered_map<uint32_t, std::set<ElementBatch::ptr>> layerToElementBatchs;
};

} // namespace ui
} // namespace re

#endif // RE_UI_ELEMENTBATCHER_H
