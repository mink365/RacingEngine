#include "LayoutElement.h"
#include <cfloat>

namespace re {

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

} // namespace re

