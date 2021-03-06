#include "HierarchyColor.h"
#include "Scene/Node.h"

namespace re {
namespace ui {

HierarchyColor::HierarchyColor()
{

}

HierarchyColor::~HierarchyColor()
{

}

void HierarchyColor::setAlpha(float a)
{
    this->color.a = a;

    this->updateColor();
}

float HierarchyColor::getAlpha() const
{
    return this->color.a;
}

void HierarchyColor::setColor(const Color &color)
{
    this->color = color;

    this->updateColor();
}

const Color &HierarchyColor::getColor() const
{
    return this->color;
}

const Color &HierarchyColor::getDisplayColor() const
{
    return this->worldColor;
}

void HierarchyColor::updateColor()
{
    auto func = [](NodePtr& node)
    {
        auto curr = node->getComponent<HierarchyColor>();
        auto parent = node->getComponentInParent<HierarchyColor>();

        if (parent) {
            curr->worldColor = curr->color * parent->getDisplayColor();
        } else {
            curr->worldColor = curr->color;
        }

    //    this->updateViewColor();
    };

    NodePtr node = getNode();
    DistpatchFunctionInHierarchy(node, func);
}

void HierarchyColor::copyProperties(const HierarchyColor &rhs)
{
    this->color = rhs.color;
    this->worldColor = rhs.worldColor;
}

} // namespace ui
} // namespace re

