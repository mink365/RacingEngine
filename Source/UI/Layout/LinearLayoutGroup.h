#ifndef RE_UI_LINEARLAYOUTGROUP_H
#define RE_UI_LINEARLAYOUTGROUP_H

#include "LayoutGroup.h"

namespace re {
namespace ui {

enum class LinearAlign {
    Left,
    Center,
    Right,
};

/**
 * @brief The LinearLayoutGroup class
 *
 * if no force expand is setted, we can align the child
 * to a point of LayoutGroup
 */
class LinearLayoutGroup : public LayoutGroup
{
public:
    LinearLayoutGroup();
    ~LinearLayoutGroup();

    void CalculateLayout();

    void SetSpacing(float v);
    void SetAlign(LinearAlign align);
    void SetExpand(bool v);

protected:
    Axis _axis;
    float _spacing;

    LinearAlign _align;
    bool _forceExpandWidth;
    bool _forceExpandHeight;
};

} // namespace ui
} // namespace re
#endif // RE_UI_LINEARLAYOUTGROUP_H
