#include "Graphic.h"
#include "Render/Geometry/Geometry.h"

namespace re {
namespace ui {

Graphic::Graphic()
{
    geometry = Geometry::create();
}

} // namespace ui
} // namespace re

