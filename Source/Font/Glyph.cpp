#include "Glyph.h"

namespace re {

Glyph::Glyph()
{
}

void Glyph::init(wchar_t c, int offsetX, int offsetY, float advanceX, float advanceY)
{
    this->charcode = c;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->advanceX = advanceX;
    this->advanceY = advanceY;
}

void Glyph::setTextureFrame(TextureFrame::ptr frame)
{
    this->frame = frame;
}

wchar_t Glyph::getCharcode()
{
    return this->charcode;
}

void Glyph::addKerningPair(const KerningPair &kerning)
{
    this->kernings.push_back(kerning);
}

void Glyph::clearKerningPair()
{
    this->kernings.clear();
}

}
