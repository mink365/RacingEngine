#ifndef RENDERQUEUE_H
#define RENDERQUEUE_H

#include <map>
#include <vector>
#include <scene/SceneNode.h>

namespace re {

/// Use this queue for objects which must be rendered first e.g. backgrounds
static const int RENDER_QUEUE_BACKGROUND = 0;

/// First queue (after backgrounds), used for skyboxes if rendered first
static const int RENDER_QUEUE_SKIES_EARLY = 5;
static const int RENDER_QUEUE_1 = 10;
static const int RENDER_QUEUE_2 = 20;
static const int RENDER_QUEUE_WORLD_GEOMETRY_1 = 25;
static const int RENDER_QUEUE_3 = 30;
static const int RENDER_QUEUE_4 = 40;
/// The default render queue
static const int RENDER_QUEUE_MAIN = 50;
static const int RENDER_QUEUE_6 = 60;
static const int RENDER_QUEUE_7 = 70;
static const int RENDER_QUEUE_WORLD_GEOMETRY_2 = 75;
static const int RENDER_QUEUE_8 = 80;
static const int RENDER_QUEUE_9 = 90;
/// Penultimate queue(before overlays), used for skyboxes if rendered last
static const int RENDER_QUEUE_SKIES_LATE = 95;
/// Use this queue for objects which must be rendered last e.g. overlays UI层
static const int RENDER_QUEUE_OVERLAY = 100;
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
    std::vector<SceneNodePtr> nodes;
};

class RenderQueue
{
    friend class RnderManager;

public:
    RenderQueue();
    virtual ~RenderQueue();

    void addRenderableNode(SceneNodePtr node);
    void addRenderableNode(SceneNodePtr node, int type);

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
