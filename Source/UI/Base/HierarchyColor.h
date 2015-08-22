#ifndef RE_UI_HIERARCHYCOLOR_H
#define RE_UI_HIERARCHYCOLOR_H

#include "Math/Color.h"
#include "Scene/Transform.h"

namespace re {
namespace ui {

class HierarchyColor;
typedef SharedPtr<HierarchyColor> HierarchyColorPtr;

class HierarchyColor : public Component
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

    ComponentPtr createCloneInstance() const;
    void copyProperties(const Component* component);

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
