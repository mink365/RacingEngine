#include "DebugRenderer.h"
#include "UI/Base/Transform2D.h"
#include "Render/Shader/ShaderManager.h"
#include "BufferObject/BufferObjectUtil.h"
#include "Scene/SceneManager.h"

namespace re {

DebugRenderer::DebugRenderer()
{
    meshData = std::make_shared<MeshData>();
    geometry = Geometry::create();

    StreamUnit unit;
    unit.format.push_back(VertexElement(VertexElementType::Position, AttributeFormat::FLOAT, 3));
    unit.format.push_back(VertexElement(VertexElementType::Diffuse, AttributeFormat::FLOAT, 4));
    meshData->vertexStreams.push_back(unit);

    auto appendFunc = [](Geometry& geometry, MeshData& meshData) {
        auto& stream = meshData.vertexStreams[0];

        stream.vertices.allocate(geometry.getVertexCount() * stream.getVertexSize());
        meshData.indices.allocate(geometry.faces.size() * 3);

        auto vertexPointer = Map<FbxVertex>(stream.vertices);
        auto facePointer = Map<Face>(meshData.indices);

        FbxVertex v;
        for (uint i = 0; i < geometry.getVertexCount(); i++) {
            v.xyz = geometry.positions[i];
            v.uv = geometry.uvs[i];
            v.normal = geometry.vertexNormals[i];
            v.color = geometry.colors[i];

            vertexPointer[i] = v;
        }

        for (uint i = 0; i < geometry.faces.size(); i++) {
            facePointer[i] = geometry.faces[i];
        }
    };
    geometry->setMeshDataAppendFunc(appendFunc);
}

DebugRenderer::~DebugRenderer()
{

}

void DebugRenderer::InitNode()
{
    node = CreateMeshNode();
    mesh = node->getComponent<Mesh>();
    mesh->setMeshData(meshData);

    auto material = node->getComponent<Material>();

    Shader::ptr shader = ShaderManager::getInstance().getShader("Shader_Debug");
    material->setShder(shader);

    material->setQueueID(RenderQueues::UI);
    material->getRenderState().depthState.depthTestEnable = false;
    material->getRenderState().depthWrite = false;
    material->getRenderState().polygonMode = PolygonMode::Line;

    SceneManager::getInstance().addRootNode(node);
}

void DebugRenderer::Step(NodePtr &node)
{
    geometry->clear();

    auto func = [&](NodePtr& node) {
        auto transform = node->getComponent<Transform2D>();
        size_t level = node->getLevel();

        this->AppendNode(transform, level);
    };

    DistpatchFunctionInHierarchy(node, func);

    geometry->appendToMeshData(meshData);

    BufferObjectUtil::getInstance().loadGeometryToHardware(*(mesh.get()));
}

void DebugRenderer::AppendNode(Transform2DPtr &transform, size_t level)
{
    float v = level / 5;
    Color color = Color(v, 1, 1, 1);

    Rect rect = transform->getBoundingBox();

    /*
     *  p4 -------- p3
     *  |           |
     *  |           |
     *  p1 -------- p2
     */
    Vec2 p1 = Vec2(rect.getMinX(), rect.getMinY());
    Vec2 p2 = Vec2(rect.getMaxX(), rect.getMinY());
    Vec2 p3 = Vec2(rect.getMaxX(), rect.getMaxY());
    Vec2 p4 = Vec2(rect.getMinX(), rect.getMaxY());

    p1 = transform->convertToWorldSpace(p1);
    p2 = transform->convertToWorldSpace(p2);
    p3 = transform->convertToWorldSpace(p3);
    p4 = transform->convertToWorldSpace(p4);

    LOG_E("Append  Level: %d, position: %f, %f", level, p1.x, p1.y);

    std::vector<Vec2> posList = {p1, p2, p3, p4};

    Vertex vertex;
    for (size_t i = 0; i < 4; ++i) {
        Vec2& p = posList[i];

        vertex.xyz = Vec3(p.x, p.y, 0);
        vertex.color = color;

        geometry->addVertex(vertex);
    }

    int faceIndex = geometry->getFaces().size() * 3 / 6;
    int offset = faceIndex * 4;
    Face face(offset + 0, offset + 1, offset + 2);
    Face face2(offset + 2, offset + 3, offset + 0);
    geometry->addFace(face);
    geometry->addFace(face2);
}

} // namespace re

