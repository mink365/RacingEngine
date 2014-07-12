#ifndef RENDERQUEUE_H
#define RENDERQUEUE_H

#include <map>
#include <vector>
#include "Scene/SceneNode.h"
#include "Scene/NodeAttribute.h"

namespace re {

/// Use this queue for objects which must be rendered first e.g. backgrounds
static const int RENDER_QUEUE_BACKGROUND = 0;
/// First queue (after backgrounds), used for skyboxes if rendered first
static const int RENDER_QUEUE_SKIES_EARLY = 5;
static const int RENDER_QUEUE_WORLD_GEOMETRY_1 = 25;
/// The default render queue
static const int RENDER_QUEUE_MAIN = 50;
static const int RENDER_QUEUE_WORLD_GEOMETRY_2 = 75;
/// Penultimate queue(before overlays), used for skyboxes if rendered last
static const int RENDER_QUEUE_SKIES_LATE = 95;
/// Use this queue for objects which must be rendered last e.g. overlays UI层
static const int RENDER_QUEUE_OVERLAY = 100;
static const int RENDER_QUEUE_UI = 103;
/// Final possible render queue, don't exceed this
static const int RENDER_QUEUE_MAX = 105;

class RenderableList
{
    friend class RenderManager;

public:
    RenderableList(const int type);

    bool operator<(const RenderableList &list) const;

public:
    int listType;
    std::vector<NodeAttributePtr> attributes;
};

class RenderQueue
{
    friend class RnderManager;

public:
    RenderQueue();
    virtual ~RenderQueue();

    void addRenderable(NodeAttributePtr node);
    void addRenderable(NodeAttributePtr node, int type);

    RenderableList &getRenderableList(int type);

    std::vector<RenderableList *> &sortRenderableList();

    void clear();

    std::map<int, RenderableList *> queues;

    /**
     * @brief list
     * 有序的渲染队列列表
     */
    std::vector<RenderableList *> lists;
};

} // namespace re

#endif // RENDERQUEUE_H
