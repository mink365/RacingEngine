#include "SceneManager.h"
#include <algorithm>

namespace re {

SceneManager::SceneManager()
{
}

void SceneManager::addRootNode(NodePtr node)
{
    node->resetParent();

    this->roots.push_back(node);
}

void SceneManager::removeRootNode(const NodePtr node)
{
    std::vector<NodePtr>::iterator iter;
    iter = find(this->roots.begin(), this->roots.end(), node);

    if (iter != this->roots.end()) {
        this->roots.erase(iter);
    }
}

void SceneManager::clearRootNodes()
{
    this->roots.clear();
}

std::vector<NodePtr> &SceneManager::getRootNodes()
{
    return this->roots;
}

bool bucketComp (const NodePtr node1, const NodePtr node2) {
    if (node1->getLevel() > node2->getLevel()) {
        return true;
    }

    return false;
}

void SceneManager::renderScene()
{
    this->clearFrame();

    for (auto node : roots) {
        this->vist(node);
    }

    sort(this->sceneTransformUpdateBucket.begin(), this->sceneTransformUpdateBucket.end(), bucketComp);
    for (auto node : sceneTransformUpdateBucket) {
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

void SceneManager::vist(const NodePtr &node)
{
    if (node->getTransform()->refreshFlags & Transform::RF_LOCAL_TRANSFORM) {
        this->sceneTransformUpdateBucket.push_back(node);
    }

    if (node->isVisible()) {
        for (auto& component : node->getComponents()) {
            if (component->getType() == ComponentType::Mesh) {
                MeshPtr mesh = dynamic_pointer_cast<Mesh>(component);
                this->renderManger.getRenderQueue().addRenderable(mesh, mesh->getMaterial()->getQueueID());
            }
        }

        for (auto& child : node->getChildren()) {
            this->vist(child);
        }
    }
}

}
