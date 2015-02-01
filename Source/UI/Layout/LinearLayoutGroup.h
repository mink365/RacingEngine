#ifndef LINEARLAYOUTGROUP_H
#define LINEARLAYOUTGROUP_H

#include "LayoutGroup.h"

namespace re
{

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

    void Layout();

protected:
    Axis _axis;
    float _spacing;

    LinearAlign _align;
    bool _forceExpandWidth;
    bool _forceExpandHeight;
};

}
#endif // LINEARLAYOUTGROUP_H
