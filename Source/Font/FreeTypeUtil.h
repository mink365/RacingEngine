#ifndef FREETYPEUTIL_H
#define FREETYPEUTIL_H

#include "Font.h"
#include "TextureAtlas.h"

namespace re {

class FreeTypeUtil
{
public:
    static void GenerateKerning(Font::ptr& font);
    static void LoadFontInfo(Font::ptr& font);

    static size_t LoadGlyphs(TextureAtlas::ptr& atlas, Font::ptr& font, const wchar_t* charcodes);
};

}

#endif // FREETYPEUTIL_H
