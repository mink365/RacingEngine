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

void RenderQueue::addRenderable(const Mat4 &mat, MaterialPtr material, MeshDataPtr meshData, int type)
{
    RenderableList &list = this->getRenderableList(type);

    Renderable element(mat, material, meshData);
    list.attributes.push_back(element);
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

        list->attributes.clear();
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
