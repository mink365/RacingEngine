#include "LayoutElement.h"
#include <cfloat>

namespace re {
namespace ui {

LayoutElement::LayoutElement()
    : fillWidth(false), fillHeight(false)
    , maxSize(FLT_MAX, FLT_MAX)
{

}

LayoutElement::~LayoutElement()
{

}

bool LayoutElement::getFillWidth()
{
    return this->fillWidth;
}

bool LayoutElement::getFillHeight()
{
    return this->fillHeight;
}

Alignment::ptr LayoutElement::getAlignment()
{
    return alignment;
}

void LayoutElement::setAlignment(Alignment::ptr &alignment)
{
    this->alignment = alignment;
}

} // namespace ui
} // namespace re

