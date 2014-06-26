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
    Font(Int size, const char * filename);

    ~Font();

public:
    Glyph::constPtr getGlyph(const wchar_t& c) const;
    void addGlyph(Glyph::ptr& glyph);

    Int getSize() const;
    FontType getType() const;

    const std::map<wchar_t, Glyph::ptr>& getAllGlyphs();

protected:
    std::map<wchar_t, Glyph::ptr> glyphs;

    /**
     * @brief size
     * font size
     */
    Int size;
    FontType type;

public:
    union {
        /**
         * Font filename, for when location == TEXTURE_FONT_FILE
         */
        char *filename;

        /**
         * Font memory address, for when location == TEXTURE_FONT_MEMORY
         */
        struct {
            const void *base;
            size_t size;
        } memory;
    };
};

}

#endif // FONT_H
