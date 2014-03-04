#include "SceneManager.h"
#include <algorithm>

SceneManager::SceneManager()
{
}

SceneManager &SceneManager::getInstance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::addRootNode(SceneNode *node)
{
    node->parent = NULL;

    this->roots.push_back(node);
}

void SceneManager::removeRootNode(const SceneNode *node)
{
    std::vector<SceneNode*>::iterator iter;
    iter = find(this->roots.begin(), this->roots.end(), node);

    if (iter != this->roots.end()) {
        this->roots.erase(iter);
    }
}

void SceneManager::clearRootNodes()
{
    this->roots.clear();
}

std::vector<SceneNode *> &SceneManager::getRootNodes()
{
    return this->roots;
}

bool bucketComp (const SceneNode *node1, const SceneNode *node2) {
    if (node1->getLevel() > node2->getLevel()) {
        return true;
    }

    return false;
}

void SceneManager::renderScene()
{
    this->clearFrame();

    std::vector<SceneNode*>::iterator iter;
    for (iter = this->roots.begin(); iter != this->roots.end(); ++iter) {
        SceneNode *node = *iter;

        this->vist(*node);
    }

    sort(this->sceneTransformUpdateBucket.begin(), this->sceneTransformUpdateBucket.end(), bucketComp);
    for (iter = this->sceneTransformUpdateBucket.begin(); iter != this->sceneTransformUpdateBucket.end(); ++iter) {
        SceneNode *node = *iter;

        node->updateTransform();
    }

    this->renderManger.render();
}

RenderManager &SceneManager::getRenderManager()
{
    return this->renderManger;
}


void SceneManager::clearFrame()
{
    this->sceneTransformUpdateBucket.clear();

    this->renderManger.clear();
}

void SceneManager::vist(SceneNode &node)
{
    if (node.refreshFlags & Node::RF_LOCAL_TRANSFORM) {
        this->sceneTransformUpdateBucket.push_back(&node);
    }

    if (node.visible) {
        // TODO: 类型辨识
        if (node.getType() == MESH) {
            Mesh *mesh = (Mesh*)&node;
            this->renderManger.getRenderQueue().addRenderableNode(*mesh, mesh->getMaterial().getQueueID());
        }

        std::vector<Node*>::iterator iter;
        for (iter = node.children.begin(); iter != node.children.end(); ++iter) {
            SceneNode *child = (SceneNode*)*iter;

            this->vist(*child);
        }
    }
}
