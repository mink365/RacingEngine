#ifndef FONT_H
#define FONT_H

#include <map>

#include "Glyph.h"

namespace re {

enum class FontType {
    File,
    Memory
};

class Font : public Named, public Shared<Font>
{
public:
    Font();

public:
    Glyph::constPtr getGlyph(const wchar_t& c) const;
    void addGlyph(Glyph::ptr& glyph);

    float getSize() const;
    FontType getType() const;

    const std::map<wchar_t, Glyph::ptr>& getAllGlyphs();

protected:
    std::map<wchar_t, Glyph::ptr> glyphs;

    /**
     * @brief size
     * font size
     */
    float size;
    FontType type;
};

}

#endif // FONT_H
