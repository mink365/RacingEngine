#ifndef RE_UI_LAYOUTGROUP_H
#define RE_UI_LAYOUTGROUP_H

#include "Math/RectOffset.h"
#include "LayoutUtil.h"

namespace re {
namespace ui {

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

} // namespace ui
} // namespace re

#endif // RE_UI_LAYOUTGROUP_H
