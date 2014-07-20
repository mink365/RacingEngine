#ifndef FONT_H
#define FONT_H

#include <map>

#include "Glyph.h"

namespace re {

class Font : public Named, public Shared<Font>
{
public:
    Font(Int size, const FilePtr& file);

    ~Font();

public:
    Glyph::constPtr getGlyph(const wchar_t& c) const;
    void addGlyph(Glyph::ptr& glyph);

    Int getSize() const;

    const std::map<wchar_t, Glyph::ptr>& getAllGlyphs();

    /**
     * @brief getTexture
     * @return
     *
     * TODO: one font can have two or more texture?
     */
    Texture::ptr getTexture();

protected:
    std::map<wchar_t, Glyph::ptr> glyphs;

    /**
     * @brief size
     * font size
     */
    Int size;

public:
    FilePtr file;
};

}

#endif // FONT_H
