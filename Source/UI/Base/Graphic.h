#ifndef RE_UI_GRAPHIC_H
#define RE_UI_GRAPHIC_H

#include "Base/ECS/Component.h"
#include "UI/Rendering/CanvasRenderElement.h"

namespace re {
namespace ui {

class Graphic : public Component
{
public:
    Graphic();

protected:
    GeometryPtr geometry;
};

} // namespace ui
} // namespace re

#endif // RE_UI_GRAPHIC_H
