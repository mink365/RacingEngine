#include "LayoutGroup.h"

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

} // namespace re

