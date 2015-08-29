#include "SceneManager.h"
#include <algorithm>
#include "RenderElement.h"
#include "UI/Rendering/ElementBatcher.h"
#include "Mesh.h"
#include "Util/ComponentFactory.h"

namespace re {

namespace ui {
    class ElementBatcher;
}

SceneManager::SceneManager()
{
    root = CreateNodeEntity()->getNode();
    root->_inScene = true;
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

    for (auto& node : root->getChildren()) {
        this->vist(node);
    }

    sort(this->sceneTransformUpdateBucket.begin(), this->sceneTransformUpdateBucket.end(), bucketComp);
    for (auto node : sceneTransformUpdateBucket) {
        node->getEntity()->refreshTransformInHierarchy();
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
    if (node->getEntity()->getTransform()->refreshFlags & Transform::RF_LOCAL_TRANSFORM) {
        this->sceneTransformUpdateBucket.push_back(node);
    }

    if (node->isVisible()) {
        auto renderElement = node->getComponent<RenderElement>();
        if (renderElement) {
            auto transform = node->getEntity()->getTransform();
            auto material = renderElement->getMaterial();
            auto mesh = renderElement->getMesh();
            this->renderManger.getRenderQueue().addRenderable(transform->getWorldMatrix(),
                                                              material, mesh->getMeshData(), material->getQueueID());
        } else if (node->getComponent<ui::ElementBatcher>()) {
            auto batcher = node->getComponent<ui::ElementBatcher>();

            batcher->Render();
        }

        for (auto& child : node->getChildren()) {
            this->vist(child);
        }
    }
}

}
