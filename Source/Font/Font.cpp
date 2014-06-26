#include "Font.h"

namespace re {

Font::Font(Int size, const char *filename)
{
    this->type = FontType::File;

    this->size = size;
    this->filename = strdup(filename);
}

Font::~Font()
{
    if (type == FontType::File && this->filename) {
        free( this->filename );
    }
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

FontType Font::getType() const
{
    return type;
}

const std::map<wchar_t, Glyph::ptr> &Font::getAllGlyphs()
{
    return this->glyphs;
}

}
