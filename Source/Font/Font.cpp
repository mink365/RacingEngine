#include "Font.h"

namespace re {

Font::Font(Int size, const FilePtr &file)
{
    this->size = size;
    this->file = file;
}

Font::~Font()
{
}

Glyph::constPtr Font::getGlyph(const wchar_t &c) const
{
    auto iter = this->glyphs.find(c);

    if (iter != this->glyphs.end()) {
        return iter->second;
    }

    return nullptr;
}

void Font::addGlyph(Glyph::ptr &glyph)
{
    this->glyphs[glyph->getCharcode()] = glyph;
}
Int Font::getSize() const
{
    return size;
}

const std::map<wchar_t, Glyph::ptr> &Font::getAllGlyphs()
{
    return this->glyphs;
}

Texture::ptr Font::getTexture()
{
    if (this->glyphs.size() > 1) {
        auto iter = this->glyphs.begin();
        TextureFrame::ptr frame = iter->second->frame;

        Texture::ptr tex = frame->getTexture();

        return tex;
    }

    return nullptr;
}

}
