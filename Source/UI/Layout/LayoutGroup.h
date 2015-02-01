#ifndef RE_LAYOUTGROUP_H
#define RE_LAYOUTGROUP_H

#include "Component.h"
#include "Math/RectOffset.h"
#include "LayoutUtil.h"

namespace re {

class LayoutGroup : public Component
{
public:
    LayoutGroup();
    ~LayoutGroup();

    std::vector<Transform2DPtr> GetChildrenTransform() const;

protected:
    RectOffset _padding;
};

} // namespace re

#endif // RE_LAYOUTGROUP_H
