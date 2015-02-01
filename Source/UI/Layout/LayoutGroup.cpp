#include "LayoutGroup.h"
#include "LayoutElement.h"

namespace re {

LayoutGroup::LayoutGroup()
{

}

LayoutGroup::~LayoutGroup()
{

}

std::vector<Transform2DPtr> LayoutGroup::GetChildrenTransform() const
{
    return std::vector<Transform2DPtr>();
}

void LayoutGroup::CalculateLayout()
{

}

void LayoutNode(NodePtr& node)
{
    auto parentTransform = node->getComponentInParent<Transform2D>();
    auto element = node->getComponent<LayoutElement>();
    auto transform = node->getComponent<Transform2D>();

    if (element->getFillWidth()) {
        transform->setWidth(parentTransform->getSize().width);
    }

    if (element->getFillHeight()) {
        transform->setHeight(parentTransform->getSize().height);
    }

    if (element->getAlignment() == nullptr) {
        auto alignment = element->getAlignment();
        LayoutUtil::LayoutToParent(transform, alignment->alignFrom, alignment->alignTo, alignment->offset);
    }
}

std::shared_ptr<LayoutGroup> GetLayoutGroup(NodePtr& node)
{
    auto& components = node->getComponents();

    for (auto& component : components) {
        auto group = std::dynamic_pointer_cast<LayoutGroup>(component);
        if (group != nullptr) {
            return group;
        }
    }

    return nullptr;
}

void LayoutRoot(NodePtr &root)
{
    auto func = [](NodePtr& node){
        auto group = GetLayoutGroup(node);
        if (group) {
            group->CalculateLayout();
        } else {
            for (auto child : node->getChildren()) {
                LayoutNode(child);
            }
        }
    };

    DistpatchFunctionInHierarchy(root, func);
}

} // namespace re

