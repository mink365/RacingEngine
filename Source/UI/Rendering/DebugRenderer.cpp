#include "DebugRenderer.h"
#include "UI/Base/Transform2D.h"
#include "Scene/RenderElement.h"
#include "BufferObject/BufferObjectUtil.h"
#include "Scene/SceneManager.h"

namespace re {

using namespace ui;

DebugRenderer::DebugRenderer()
{
    meshData = Create<MeshData>();
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
    mesh = node->getComponent<RenderElement>()->getMesh();
    mesh->setMeshData(meshData);

    auto material = node->getComponent<RenderElement>()->getMaterial();

    Shader::ptr shader = ShaderManager::instance().GetResource("Shader_Debug");
    material->setShder(shader);

    material->setQueueID(RenderQueues::UI);
    material->getRenderState().depthState.depthTestEnable = false;
    material->getRenderState().depthWrite = false;
    material->getRenderState().polygonMode = PolygonMode::Fill;
    material->getRenderState().faceCullMode = FaceCullMode::Off;

    SceneManager::instance().addRootNode(node->getNode());
}

void DebugRenderer::Step(NodePtr &node)
{
    geometry->clear();

    auto func = [&](NodePtr& node) {
        if (!node->isVisible()) {
            return;
        }

        auto transform = node->getComponent<Transform2D>();
        size_t level = node->getLevel();

        this->AppendNode(transform, level);
    };

    DistpatchFunctionInHierarchy(node, func);

    geometry->appendToMeshData(meshData);

    BufferObjectUtil::instance().loadGeometryToHardware(*(meshData.get()));
}

void DebugRenderer::AppendNode(Transform2DPtr &transform, size_t level)
{
    int v = level / 12.0f * std::stoi("FFFFFF", 0, 16);

    char buf[7];
    std::sprintf(buf, "%06X", v);

    Color color = Color::FromHex(buf);

    /*
     *  p4 -------- p3
     *  |           |
     *  |           |
     *  p1 -------- p2
     */
    Vec2 p1 = Vec2(0, 0);
    Vec2 p2 = Vec2(transform->getSize().width, 0);
    Vec2 p3 = Vec2(transform->getSize().width, transform->getSize().height);
    Vec2 p4 = Vec2(0, transform->getSize().height);

    std::vector<Vec2> posList = {p1, p2, p3, p4};

    currTransform = transform;
    AddLines(posList, color);
}

void DebugRenderer::AddLines(const std::vector<Vec2> &points, const Color &color)
{
    const float radius = 0.5f;
    // TODO: from screen space to world
    float requestedThickess = MathLib::max(1.0f, 0.5f);
    float lineThickess = MathLib::ceil( (2.0f * radius + requestedThickess) * MathLib::sqrt(2.0f) );
    float halfThickess = lineThickess * 0.5f + radius;

    Vec2 startPoint, endPoint, normal, up;

    startPoint = points[0];
    endPoint = points[1];
    normal = (Vec3(endPoint - startPoint).cross(Vec3(0, 0, 1))).normalize();
    up = normal * halfThickess;

    AddVertex(startPoint + up, color);
    AddVertex(startPoint - up, color);

    for (size_t i = 1; i < points.size(); ++i) {
        endPoint = points[i];

        if (i > 1) {

        }

        AddLine(startPoint, endPoint, color, halfThickess, false);

        startPoint = endPoint;
    }

    AddLine(startPoint, points[0], color, halfThickess, false);
}

void DebugRenderer::AddLine(const Vec2 &startPoint, const Vec2 &endPoint, const Color &color, float halfThickess, bool loop)
{
    Vec3 normal = (Vec3(endPoint - startPoint).cross(Vec3(0, 0, 1))).normalize();
    Vec3 __up = normal * halfThickess;
    Vec2 up = Vec2(__up.x, __up.y);

    Vec2 intersectUpper = endPoint + up;
    Vec2 intersectLower = endPoint - up;
//        Vec2 intersectCenter = endPoint;

    auto startIndex = geometry->getVertexCount();
    if (loop) {
        Vertex startV1 = geometry->getVertex(startIndex - 1);
        Vertex startV2 = geometry->getVertex(startIndex - 2);

        // TODO: change the uv

        geometry->addVertex(startV2);
        geometry->addVertex(startV1);
    } else {
        AddVertex(startPoint + up, color);
        AddVertex(startPoint - up, color);
    }
    startIndex += 2;

    AddVertex(intersectUpper, color);
    AddVertex(intersectLower, color);

    geometry->addFace({startIndex - 1, startIndex - 2, startIndex + 0});
    geometry->addFace({startIndex + 0, startIndex + 1, startIndex - 1});
}

void DebugRenderer::AddVertex(const Vec2 &point, const Color &color)
{
    Vec2 p1 = currTransform->convertToWorldSpace(point);

    Vertex vertex;
    vertex.xyz = p1;
    vertex.color = color;

    geometry->addVertex(vertex);
}

} // namespace re

