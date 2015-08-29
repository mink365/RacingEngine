#ifndef RE_UI_CANVAS_H
#define RE_UI_CANVAS_H

#include "Base/ECS/Component.h"

namespace re {
namespace ui {

/**
 * @brief The Canvas class
 * Canvas is the base class of ui rendering
 */
class Canvas : public Component
{
public:
    Canvas();
};

} // namespace ui
} // namespace re

#endif // RE_UI_CANVAS_H
