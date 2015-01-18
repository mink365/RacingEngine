#include "SceneManager.h"
#include <algorithm>

namespace re {

SceneManager::SceneManager()
{
}

void SceneManager::addRootNode(NodePtr node)
{
    node->resetParent();

    root->addChild(node);
}

void SceneManager::removeRootNode(const NodePtr node)
{
    root->removeChild(node);
}

void SceneManager::clearRootNodes()
{
    root->removeAllChildren();
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

    DistpatchFunctionInHierarchy(root, [](NodePtr& node){
        node->Update();
    });

    for (auto& node : root->getChildren()) {
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
