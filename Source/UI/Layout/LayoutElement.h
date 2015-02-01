#ifndef RE_LAYOUTELEMENT_H
#define RE_LAYOUTELEMENT_H

#include "Component.h"
#include "Math/Size.h"
#include "LayoutUtil.h"

namespace re {

class Alignment : public Shared<Alignment>
{
public:
    Vec2 alignFrom;
    Vec2 alignTo;
    Vec2 offset;
};

class LayoutElement : public Component
{
public:
    LayoutElement();
    ~LayoutElement();

    bool getFillWidth();
    bool getFillHeight();
    Alignment::ptr getAlignment();

    void setAlignment(Alignment::ptr& alignment);

protected:
    Size preferredSize;
    Size minSize;
    Size maxSize;

    bool fillWidth;
    bool fillHeight;

    Alignment::ptr alignment;
};

} // namespace re

#endif // RE_LAYOUTELEMENT_H
