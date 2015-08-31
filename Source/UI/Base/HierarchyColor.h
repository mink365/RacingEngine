#ifndef RE_UI_HIERARCHYCOLOR_H
#define RE_UI_HIERARCHYCOLOR_H

#include "Math/Color.h"
#include "Scene/Transform.h"

namespace re {
namespace ui {

class HierarchyColor;
typedef ComponentHandle<HierarchyColor> HierarchyColorPtr;

class HierarchyColor : public Component<HierarchyColor>
{
public:
    HierarchyColor();
    ~HierarchyColor();

public:
    void setAlpha(float a);
    float getAlpha() const;

    void setColor(const Color& color);
    const Color& getColor() const;

    const Color& getDisplayColor() const;

protected:
    void updateColor();

    void copyProperties(const HierarchyColor& rhs);

protected:
    Color color;

    /**
     * @brief worldColor
     */
    Color worldColor;
};

} // namespace ui
} // namespace re

#endif // RE_UI_HIERARCHYCOLOR_H
