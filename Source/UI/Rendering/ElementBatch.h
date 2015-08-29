#ifndef RE_UI_ELEMENTBATCH_H
#define RE_UI_ELEMENTBATCH_H

#include "CanvasRenderElement.h"

namespace re {

class MeshData;
using MeshDataPtr = SharedPtr<MeshData>;

namespace ui {

class ElementBatch : public Shared<ElementBatch>
{
public:
    ElementBatch();

    void Clear();
    void AddElement(const CanvasRenderElement& element);
    void FillMeshData();

    MaterialPtr getMaterial() const;
    TexturePtr getTexture() const;
    MeshDataPtr getMeshData() const;

private:
    MaterialPtr material;
    TexturePtr texture;

    MeshDataPtr meshData;
    GeometryPtr geometry;
};

} // namespace ui
} // namespace re

#endif // RE_UI_ELEMENTBATCH_H
