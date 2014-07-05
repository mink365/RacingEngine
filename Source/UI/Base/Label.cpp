#include "Label.h"

#include "Font/TextStuffer.h"
#include "Render/BufferObject/BufferObjectUtil.h"

namespace re {

Label::Label()
{
}

void Label::init(Font::ptr &font)
{
    this->font = font;
}

void Label::setText(const string &text)
{
    SceneNodePtr node = std::dynamic_pointer_cast<SceneNode>(this->shared_from_this());
    InitNodeForLeaf(node, font->getTexture());

    TextStuffer::getInstance().AddText(std::wstring(text.begin(), text.end()), this->getGeometry(), this->font);

//    // TODO: set the size and anchorPoint pixel

    BufferObjectUtil::getInstance().loadGeometryToHardware(*(this->getGeometry().get()));
}

}
