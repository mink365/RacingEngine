#include "ElementBatch.h"
#include "Render/Geometry/Geometry.h"
#include "UI/Base/Transform2D.h"
#include "Render/Mesh.h"
#include "Render/BufferObject/BufferObjectUtil.h"

namespace re {
namespace ui {

ElementBatch::ElementBatch()
{
    this->geometry = Geometry::create();

    meshData = Create<MeshData>();

    StreamUnit unit;
    unit.format.push_back(VertexElement(VertexElementType::Position, AttributeFormat::FLOAT, 3));
    unit.format.push_back(VertexElement(VertexElementType::TextureCoord, AttributeFormat::FLOAT, 2));
    unit.format.push_back(VertexElement(VertexElementType::Normal, AttributeFormat::FLOAT, 3));
    unit.format.push_back(VertexElement(VertexElementType::Diffuse, AttributeFormat::FLOAT, 4));
    meshData->vertexStreams.push_back(unit);
}

void ElementBatch::Clear()
{
    this->geometry->clear();
}

void ElementBatch::AddElement(const CanvasRenderElement &element)
{
    auto elementGeometry = element.getGeometry();
    auto& worldMatrix = element.getComponent<Transform2D>()->getWorldMatrix();

    int vertexCount = elementGeometry->getVertexCount();
    for (int i = 0; i < vertexCount; ++i) {
        Vertex vertex = elementGeometry->getVertex(i);

        Vec3 worldP = worldMatrix * vertex.xyz;
        vertex.xyz = worldP;

        this->geometry->addVertex(vertex);
    }

    for (auto& face : elementGeometry->getFaces()) {
        this->geometry->addFace(face);
    }

    this->material = element.getMaterial();
    this->texture = element.getTexture();
}

void ElementBatch::FillMeshData()
{
    this->geometry->appendToMeshData(this->meshData);

    BufferObjectUtil::instance().loadGeometryToHardware(*(meshData.get()));
}

MaterialPtr ElementBatch::getMaterial() const
{
    return material;
}

TexturePtr ElementBatch::getTexture() const
{
    return texture;
}

MeshDataPtr ElementBatch::getMeshData() const
{
    return meshData;
}

} // namespace ui
} // namespace re

