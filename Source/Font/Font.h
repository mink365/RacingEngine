#ifndef FONT_H
#define FONT_H

#include <map>

#include "Glyph.h"

namespace re {

/**
 * @brief The FontType enum
 * different type of font have different depth and texture format, they
 * will use different shader normally
 *
 * TTF: generated from ttf on the fly, depth is 1, texture formate TextureFormat::RED
 * Bitmap: generated by GlyphDesigner and so on, depth is 4, texture formate TextureFormat::RGBA8
 */
enum class FontType
{
    TTF,
    Bitmap,
};

class Font : public Named, public Shared<Font>
{
    friend class FreeTypeUtil;
    friend class TextStuffer;

public:
    Font(FontType type, Int size, const FilePtr& file);

    ~Font();

public:
    Glyph::constPtr getGlyph(const wchar_t& c) const;
    void addGlyph(Glyph::ptr& glyph);

    FontType getType() const;
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
    FontType type;

    /**
     * @brief size
     * font size, size of glyph
     */
    Int size;

    std::map<wchar_t, Glyph::ptr> glyphs;

    /**
     * This field is simply used to compute a default line spacing (i.e., the
     * baseline-to-baseline distance) when writing text with this font. Note
     * that it usually is larger than the sum of the ascender and descender
     * taken as absolute values. There is also no guarantee that no glyphs
     * extend above or below subsequent baselines when using this distance.
     */
    float height;

    /**
     * This field is the distance that must be placed between two lines of
     * text. The baseline-to-baseline distance should be computed as:
     * ascender - descender + linegap
     */
    float linegap;

    /**
     * The ascender is the vertical distance from the horizontal baseline to
     * the highest 'character' coordinate in a font face. Unfortunately, font
     * formats define the ascender differently. For some, it represents the
     * ascent of all capital latin characters (without accents), for others it
     * is the ascent of the highest accented character, and finally, other
     * formats define it as being equal to bbox.yMax.
     */
    float ascender;

    /**
     * The descender is the vertical distance from the horizontal baseline to
     * the lowest 'character' coordinate in a font face. Unfortunately, font
     * formats define the descender differently. For some, it represents the
     * descent of all capital latin characters (without accents), for others it
     * is the ascent of the lowest accented character, and finally, other
     * formats define it as being equal to bbox.yMin. This field is negative
     * for values below the baseline.
     */
    float descender;

    /**
     * The position of the underline line for this face. It is the center of
     * the underlining stem. Only relevant for scalable formats.
     */
    float underline_position;

    /**
     * The thickness of the underline for this face. Only relevant for scalable
     * formats.
     */
    float underline_thickness;

public:
    FilePtr file;
};

}

#endif // FONT_H
