#include "RenderQueue.h"

namespace re {

RenderQueue::RenderQueue()
{
}

RenderQueue::~RenderQueue()
{
    std::map<int, RenderableList *>::iterator iter;
    for (iter = this->queues.begin(); iter != this->queues.end(); ++iter) {
        RenderableList *list = iter->second;

        delete list;
    }
}

void RenderQueue::addRenderableNode(SceneNodePtr node)
{
    this->addRenderableNode(node, RENDER_QUEUE_MAIN);
}

void RenderQueue::addRenderableNode(SceneNodePtr node, int type)
{
    RenderableList &list = this->getRenderableList(type);

    list.nodes.push_back(node);
}

RenderableList &RenderQueue::getRenderableList(int type)
{
    if (this->queues.count(type) == 0) {
        this->queues[type] = new RenderableList(type);
    }

    RenderableList* list = this->queues[type];
    return *list;
}

std::vector<RenderableList *> &RenderQueue::sortRenderableList()
{
    this->lists.clear();

    std::map<int, RenderableList *>::iterator iter;
    for (iter = this->queues.begin(); iter != this->queues.end(); ++iter) {
        RenderableList *list = iter->second;

        this->lists.push_back(list);
    }

    return this->lists;
}

void RenderQueue::clear()
{
    std::map<int, RenderableList *>::iterator iter;
    for (iter = this->queues.begin(); iter != this->queues.end(); ++iter) {
        RenderableList *list = iter->second;

        list->nodes.clear();
    }
}

RenderableList::RenderableList(const int type)
{
    this->listType = type;
}

bool RenderableList::operator <(const RenderableList &list) const
{
    return this->listType < list.listType;
}

}
