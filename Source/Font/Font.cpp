#include "Font.h"

namespace re {

Font::Font()
{
}

Glyph::constPtr Font::getGlyph(const wchar_t &c) const
{
    auto iter = this->glyphs.find(c);

    if (iter != this->glyphs.end()) {
        // TODO:
//        return *iter->second;
    }

    return nullptr;
}
float Font::getSize() const
{
    return size;
}

FontType Font::getType() const
{
    return type;
}

}
