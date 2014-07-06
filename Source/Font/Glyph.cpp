#include "Glyph.h"

#include <algorithm>

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

float Glyph::getKerning(wchar_t charcode) const
{
    auto iter = std::find_if(kernings.begin(), kernings.end(), [=](const KerningPair& pair) {
        if (pair.charcode == charcode) {
            return true;
        }

        return false;
    });

    if (iter != kernings.end()) {
        return (*iter).kerning;
    }

    return 0;
}

void Glyph::addKerningPair(const KerningPair &kerning)
{
    this->kernings.push_back(kerning);
}

void Glyph::clearKerningPair()
{
    this->kernings.clear();
}

float Glyph::getWidth() const
{
    return this->frame->getSize().width;
}

float Glyph::getHeight() const
{
    return this->frame->getSize().height;
}

}
