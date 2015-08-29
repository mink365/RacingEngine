#ifndef RE_DEBUGRENDERER_H
#define RE_DEBUGRENDERER_H

#include "Base/Singleton.h"
#include "Scene/Mesh.h"

namespace re {

class DebugRenderer : public Singleton<DebugRenderer>
{
public:
    DebugRenderer();
    ~DebugRenderer();

    void InitNode();

    void Step(NodePtr &node);

private:
    void AppendNode(ui::Transform2DPtr& transform, size_t level);

    void AddLines(const std::vector<Vec2>& points, const Color& color, bool loop);
    void AddLine(const Vec2& startPoint, const Vec2& endPoint, const Color& color, float halfThickess, bool loop);
    void AddVertex(const Vec2& point, const Color& color);

private:
    MeshPtr mesh;
    MeshDataPtr meshData;
    GeometryPtr geometry;

    EntityPtr node;

    ui::Transform2DPtr currTransform;
};

} // namespace re

#endif // RE_DEBUGRENDERER_H
