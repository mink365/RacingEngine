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

private:
    MeshPtr mesh;
    MeshDataPtr meshData;
    GeometryPtr geometry;

    NodePtr node;
};

} // namespace re

#endif // RE_DEBUGRENDERER_H
