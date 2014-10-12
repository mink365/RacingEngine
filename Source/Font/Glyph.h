#ifndef GLYPH_H
#define GLYPH_H

#include <stdlib.h>
#include <vector>

#include "Texture/Frame/TextureFrame.h"

namespace re {

struct KerningPair
{
    /**
     * Left character code in the kern pair.
     */
    wchar_t charcode;

    /**
     * Kerning value (in fractional pixels).
     */
    float kerning;
};

/**
 *
 * A structure that describe a glyph.
 *
 *
 * Glyph metrics:
 * --------------
 *
 *                       xmin                     xmax
 *                        |                         |
 *                        |<-------- width -------->|
 *                        |                         |
 *              |         +-------------------------+----------------- ymax
 *              |         |    ggggggggg   ggggg    |     ^        ^
 *              |         |   g:::::::::ggg::::g    |     |        |
 *              |         |  g:::::::::::::::::g    |     |        |
 *              |         | g::::::ggggg::::::gg    |     |        |
 *              |         | g:::::g     g:::::g     |     |        |
 *    offset_x -|-------->| g:::::g     g:::::g     |  offset_y    |
 *              |         | g:::::g     g:::::g     |     |        |
 *              |         | g::::::g    g:::::g     |     |        |
 *              |         | g:::::::ggggg:::::g     |     |        |
 *              |         |  g::::::::::::::::g     |     |      height
 *              |         |   gg::::::::::::::g     |     |        |
 *  baseline ---*---------|---- gggggggg::::::g-----*--------      |
 *            / |         |             g:::::g     |              |
 *     origin   |         | gggggg      g:::::g     |              |
 *              |         | g:::::gg   gg:::::g     |              |
 *              |         |  g::::::ggg:::::::g     |              |
 *              |         |   gg:::::::::::::g      |              |
 *              |         |     ggg::::::ggg        |              |
 *              |         |         gggggg          |              v
 *              |         +-------------------------+----------------- ymin
 *              |                                   |
 *              |------------- advance_x ---------->|
 */

class Glyph : public Shared<Glyph>
{
public:
    Glyph();

    void init(wchar_t c, int offsetX, int offsetY, float advanceX, float advanceY);
    void setTextureFrame(TextureFrame::ptr frame);

    wchar_t getCharcode();

    float getKerning(wchar_t charcode) const;

    void addKerningPair(const KerningPair& kerning);
    void clearKerningPair();

    float getWidth() const;
    float getHeight() const;

public:
    /**
     * Wide character this glyph represents
     */
    wchar_t charcode;

    /**
     * Glyph's left bearing expressed in integer pixels.
     */
    int offsetX;

    /**
     * Glyphs's top bearing expressed in integer pixels.
     *
     * Remember that this is the distance from the baseline to the top-most
     * glyph scanline, upwards y coordinates being positive.
     */
    int offsetY;

    /**
     * For horizontal text layouts, this is the horizontal distance (in
     * fractional pixels) used to increment the pen position when the glyph is
     * drawn as part of a string of text.
     */
    float advanceX;

    /**
     * For vertical text layouts, this is the vertical distance (in fractional
     * pixels) used to increment the pen position when the glyph is drawn as
     * part of a string of text.
     */
    float advanceY;

    std::vector<KerningPair> kernings;

    TextureFrame::ptr frame;
};

}

#endif // GLYPH_H
