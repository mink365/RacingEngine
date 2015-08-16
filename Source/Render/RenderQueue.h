#ifndef RENDERQUEUE_H
#define RENDERQUEUE_H

#include <map>
#include <vector>
#include "Renderable.h"

namespace re {

namespace RenderQueues
{
    // Use this queue for objects which must be rendered first e.g. backgrounds
    const int Background  = 1000;
    // The default render queue
    const int Geometry    = 2000;
    const int AlphaTest   = 3000;
    const int Transparent = 4000;
    // Use this queue for objects which must be rendered last e.g. overlays
    const int Overlay     = 5000;
    const int UI          = 6000;
}

class RenderableList
{
    friend class RenderManager;

public:
    RenderableList(const int type);

    bool operator<(const RenderableList &list) const;

public:
    int listType;
    std::vector<Renderable> attributes;
};

class RenderQueue
{
    friend class RnderManager;

public:
    RenderQueue();
    virtual ~RenderQueue();

    void addRenderable(const Mat4& mat, MaterialPtr material, MeshDataPtr meshData, int type);

    RenderableList &getRenderableList(int type);

    std::vector<RenderableList *> &sortRenderableList();

    void clear();

    std::map<int, RenderableList *> queues;

    /**
     * @brief list
     * renderable lists with order
     */
    std::vector<RenderableList *> lists;
};

} // namespace re

#endif // RENDERQUEUE_H
