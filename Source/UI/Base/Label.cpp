#include "Label.h"

#include "Font/TextStuffer.h"

namespace re {

Label::Label()
{
}

void Label::setText(string &text)
{
    TextStuffer::getInstance().AddText(std::wstring(text.begin(), text.end()), this->getGeometry(), this->font);

    // TODO: set the size and anchorPoint pixel
}

}
