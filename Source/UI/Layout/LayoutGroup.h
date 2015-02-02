#ifndef RE_LAYOUTGROUP_H
#define RE_LAYOUTGROUP_H

#include "Component.h"
#include "Math/RectOffset.h"
#include "LayoutUtil.h"

namespace re {

void LayoutRoot(NodePtr& root);

class LayoutGroup : public Component
{
public:
    LayoutGroup();
    virtual ~LayoutGroup();

    std::vector<Transform2DPtr> GetChildrenTransform() const;

    void virtual CalculateLayout();

protected:
    RectOffset _padding;
};

} // namespace re

#endif // RE_LAYOUTGROUP_H
