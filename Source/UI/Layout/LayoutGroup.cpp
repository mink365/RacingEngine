#include "LayoutGroup.h"
#include "LayoutElement.h"
#include "Scene/Node.h"

namespace re {
namespace ui {

LayoutGroup::LayoutGroup()
    : _padding(5, 5, 5, 5)
{

}

LayoutGroup::~LayoutGroup()
{

}

std::vector<Transform2DPtr> LayoutGroup::GetChildrenTransform() const
{
    auto list =  std::vector<Transform2DPtr>();

    for (auto& child : getNode()->getChildren()) {
        list.push_back(child->getEntity()->getComponent<Transform2D>());
    }

    return list;
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

    auto alignment = element->getAlignment();
    if (alignment != nullptr) {
        LayoutUtil::LayoutToParent(transform, alignment->alignFrom, alignment->alignTo, alignment->offset);
    }
}

void LayoutRoot(NodePtr &root)
{
    auto func = [](NodePtr node){
        auto group = node->getComponent<LayoutGroup>();
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

} // namespace ui
} // namespace re

