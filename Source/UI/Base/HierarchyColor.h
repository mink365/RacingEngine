#ifndef HIERARCHYCOLOR_H
#define HIERARCHYCOLOR_H

#include "Math/Color.h"
#include "Scene/Transform.h"

namespace re
{

class HierarchyColor;
typedef std::shared_ptr<HierarchyColor> HierarchyColorPtr;

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

}

#endif // HIERARCHYCOLOR_H
