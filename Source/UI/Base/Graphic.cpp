#include "Graphic.h"
#include "Render/Geometry/Geometry.h"

namespace re {
namespace ui {

Graphic::Graphic()
{
    geometry = Geometry::create();
}

void Graphic::onAwake()
{
    auto element = this->getComponent<CanvasRenderElement>();
    element->setGeometry(geometry);
}

void Graphic::registerEvents()
{
    RegisterEvent(Events::Awake, this, &Graphic::onAwake);
}

} // namespace ui
} // namespace re

