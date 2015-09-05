#include "ElementBatcher.h"
#include "Scene/Node.h"
#include "SceneManager.h"
#include "ElementBatch.h"

namespace re {
namespace ui {

ElementBatcher::ElementBatcher()
{

}

void ElementBatcher::ReBuild()
{
    this->elementBatchs.clear();

    typedef ComponentHandle<CanvasRenderElement> CanvasRenderElementPtr;
    auto list = std::vector<CanvasRenderElementPtr>();

    auto root = this->getNode();
    DistpatchFunctionInHierarchy(root, [&](NodePtr& node){
        auto element = node->getComponent<CanvasRenderElement>();

        if (element) {
            list.push_back(element);
        }
    });

    for (auto& element : list) {
        auto& ref = *(element.get());
        auto batch = FindBatchForElement(ref);

        batch->AddElement(ref);
    }
}

void ElementBatcher::Render()
{
    for (auto& batch : this->elementBatchs) {
        batch->Clear();
    }

//    this->ReBuild();

    auto& renderQueue = SceneManager::instance().getRenderManager().getRenderQueue();

    Mat4 identity = Mat4::Identity;
    for (auto& batch : this->elementBatchs) {
        batch->FillMeshData();

        renderQueue.addRenderable(identity, batch->getMaterial(), batch->getMeshData(), RenderQueues::UI);
    }
}

ElementBatch::ptr ElementBatcher::FindBatchForElement(const CanvasRenderElement &element)
{
    //TODO: auto batch

    auto batch = ElementBatch::create();
    elementBatchs.push_back(batch);

    return batch;
}

} // namespace ui
} // namespace re

