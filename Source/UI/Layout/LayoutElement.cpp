#include "LayoutElement.h"
#include <cfloat>

namespace re {
namespace ui {

LayoutElement::LayoutElement()
    : maxSize(FLT_MAX, FLT_MAX)
      ,fillWidth(false), fillHeight(false)
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

