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

} // namespace re

