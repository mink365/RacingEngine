#ifndef RE_UI_ELEMENTBATCH_H
#define RE_UI_ELEMENTBATCH_H

#include "PreDeclare.h"
#include "CanvasRenderElement.h"

namespace re {
namespace ui {

class ElementBatch : public Shared<ElementBatch>
{
public:
    ElementBatch();

    void AddElement(const CanvasRenderElement& element);

private:
    MaterialPtr material;
    TexturePtr texture;

    MeshDataPtr meshData;
    GeometryPtr geometry;
};

} // namespace ui
} // namespace re

#endif // RE_UI_ELEMENTBATCH_H
