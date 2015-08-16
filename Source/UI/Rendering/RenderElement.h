#ifndef RE_UI_RENDERELEMENT_H
#define RE_UI_RENDERELEMENT_H

#include "PreDeclare.h"
#include "Component.h"

namespace re {
namespace ui {

class RenderElement : public Component
{
public:
    RenderElement();

private:
    MaterialPtr material;
    TexturePtr texture;

    GeometryPtr geometry;
};

} // namespace ui
} // namespace re

#endif // RE_UI_RENDERELEMENT_H
