#include "HierarchyColor.h"

namespace re
{

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
    auto node = this->getNode();

    auto parent = getComponentInParent<HierarchyColor>();

    if (parent) {
        worldColor = color * parent->getDisplayColor();
    } else {
        worldColor = color;
    }

//    this->updateViewColor();

    for (auto& child : node->getChildren()) {
        auto node = child->getComponent<HierarchyColor>();

        if (node) {
            node->updateColor();
        }
    }
}

ComponentPtr HierarchyColor::createCloneInstance() const
{
    return CreateCloneInstance<HierarchyColor>();
}

void HierarchyColor::copyProperties(const Component* node)
{
    Component::copyProperties(node);

    const HierarchyColor* inst = dynamic_cast<const HierarchyColor*>(node);
    if (inst) {
        this->color = inst->color;
        this->worldColor = inst->worldColor;
    }
}

}

