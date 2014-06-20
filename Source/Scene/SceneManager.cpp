#include "SceneManager.h"
#include <algorithm>

namespace re {

SceneManager::SceneManager()
{
}

void SceneManager::addRootNode(SceneNodePtr node)
{
    node->resetParent();

    this->roots.push_back(node);
}

void SceneManager::removeRootNode(const SceneNodePtr node)
{
    std::vector<SceneNodePtr>::iterator iter;
    iter = find(this->roots.begin(), this->roots.end(), node);

    if (iter != this->roots.end()) {
        this->roots.erase(iter);
    }
}

void SceneManager::clearRootNodes()
{
    this->roots.clear();
}

std::vector<SceneNodePtr> &SceneManager::getRootNodes()
{
    return this->roots;
}

bool bucketComp (const SceneNodePtr node1, const SceneNodePtr node2) {
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

void SceneManager::vist(const SceneNodePtr &node)
{
    if (node->refreshFlags & Node::RF_LOCAL_TRANSFORM) {
        this->sceneTransformUpdateBucket.push_back(node);
    }

    if (node->visible) {
        if (node->getNodeAttribute() != nullptr
                &&node->getNodeAttribute()->getType() == NodeAttributeType::Mesh) {
            MeshPtr mesh = dynamic_pointer_cast<Mesh>(node->getNodeAttribute());
            this->renderManger.getRenderQueue().addRenderable(mesh, mesh->getMaterial()->getQueueID());
        }

        for (auto child : node->children) {
            auto childSceneNode = dynamic_pointer_cast<SceneNode>(child);

            this->vist(childSceneNode);
        }
    }
}

}
