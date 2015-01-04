#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
// #include FT_ADVANCES_H
#include FT_LCD_FILTER_H

#include "FreeTypeUtil.h"
#include "FileSystem/File.h"

namespace re {

#define HRES  64
#define HRESf 64.f
#define DPI   72

#undef __FTERRORS_H__
#define FT_ERRORDEF( e, v, s )  { e, s },
#define FT_ERROR_START_LIST     {
#define FT_ERROR_END_LIST       { 0, 0 } };
const struct {
    int          code;
    const char*  message;
} FT_Errors[] =
#include FT_ERRORS_H

static ByteBufferPtr buf = nullptr;
static int LoadFace(Font::ptr& font, float size, FT_Library *library, FT_Face *face)
{
    FT_Error error;
    FT_Matrix matrix = {
        (int)((1.0/HRES) * 0x10000L),
        (int)((0.0)      * 0x10000L),
        (int)((0.0)      * 0x10000L),
        (int)((1.0)      * 0x10000L)};

    assert(library);
    assert(size);

    /* Initialize library */
    error = FT_Init_FreeType(library);
    if(error) {
        fprintf(stderr, "FT_Error (0x%02x) : %s\n",
                FT_Errors[error].code, FT_Errors[error].message);
        return 0;
    }

    /* Load face */
    buf = font->file->read();
    error = FT_New_Memory_Face(*library, (const FT_Byte*)buf->getData(), buf->getSize(), 0, face);

    if(error) {
        fprintf(stderr, "FT_Error (line %d, code 0x%02x) : %s\n",
                __LINE__, FT_Errors[error].code, FT_Errors[error].message);
        FT_Done_FreeType(*library);
        return 0;
    }

    /* Select charmap */
    error = FT_Select_Charmap(*face, FT_ENCODING_UNICODE);
    if(error) {
        fprintf(stderr, "FT_Error (line %d, code 0x%02x) : %s\n",
                __LINE__, FT_Errors[error].code, FT_Errors[error].message);
        FT_Done_Face(*face);
        FT_Done_FreeType(*library);
        return 0;
    }

    /* Set char size */
    error = FT_Set_Char_Size(*face, (int)(size * HRES), 0, DPI * HRES, DPI);

    if(error) {
        fprintf(stderr, "FT_Error (line %d, code 0x%02x) : %s\n",
                __LINE__, FT_Errors[error].code, FT_Errors[error].message);
        FT_Done_Face(*face);
        FT_Done_FreeType(*library);
        return 0;
    }

    /* Set transform matrix */
    FT_Set_Transform(*face, &matrix, NULL);

    return 1;
}

static int GetFaceWithSize(Font::ptr& font, float size, FT_Library *library, FT_Face *face)
{
    return LoadFace(font, size, library, face);
}

static int texture_font_get_face(Font::ptr& font, FT_Library *library, FT_Face *face)
{
    return GetFaceWithSize(font, font->getSize(), library, face);
}

static int texture_font_get_hires_face(Font::ptr& font, FT_Library *library, FT_Face *face)
{
    return GetFaceWithSize(font, font->getSize() * 100.f, library, face);
}

void FreeTypeUtil::GenerateKerning(Font::ptr& font)
{
    FT_Library library;
    FT_Face face;
    FT_UInt glyph_index, prev_index;
    FT_Vector kerning;

    /* Load font */
    if(!texture_font_get_face(font, &library, &face))
        return;

    /* For each glyph couple combination, check if kerning is necessary */
    /* Starts at index 1 since 0 is for the special backgroudn glyph */
    for(auto pair : font->getAllGlyphs())
    {
        auto glyph = pair.second;
        glyph_index = FT_Get_Char_Index( face, glyph->getCharcode() );
        glyph->clearKerningPair();

        for( auto pair : font->getAllGlyphs())
        {
            auto prev_glyph = pair.second;
            prev_index = FT_Get_Char_Index( face, prev_glyph->getCharcode() );
            FT_Get_Kerning( face, prev_index, glyph_index, FT_KERNING_UNFITTED, &kerning );
            // printf("%c(%d)-%c(%d): %ld\n",
            //       prev_glyph->charcode, prev_glyph->charcode,
            //       glyph_index, glyph_index, kerning.x);
            if( kerning.x )
            {
                KerningPair k = {prev_glyph->getCharcode(), kerning.x / (float)(HRESf*HRESf)};
                glyph->addKerningPair(k);
            }
        }
    }

    FT_Done_Face( face );
    FT_Done_FreeType( library );
    buf.reset();
}

void FreeTypeUtil::LoadFontInfo(Font::ptr &font)
{
    FT_Library library;
    FT_Face face;
    FT_Size_Metrics metrics;

    /* Get font metrics at high resolution */
    if(!texture_font_get_hires_face(font, &library, &face))
        return;

    font->underline_position = face->underline_position / (float)(HRESf*HRESf) * font->size;
    font->underline_position = round( font->underline_position );
    if( font->underline_position > -2 )
    {
        font->underline_position = -2.0;
    }

    font->underline_thickness = face->underline_thickness / (float)(HRESf*HRESf) * font->size;
    font->underline_thickness = round( font->underline_thickness );
    if( font->underline_thickness < 1 )
    {
        font->underline_thickness = 1.0;
    }

    metrics = face->size->metrics;
    font->ascender = (metrics.ascender >> 6) / 100.0;
    font->descender = (metrics.descender >> 6) / 100.0;
    font->height = (metrics.height >> 6) / 100.0;
    font->linegap = font->height - font->ascender + font->descender;

    FT_Done_Face( face );
    FT_Done_FreeType( library );

}

size_t FreeTypeUtil::LoadGlyphs(TextureAtlas::ptr& atlas, Font::ptr& font, const wchar_t *charcodes)
{
    size_t width, height, depth, w, h;
    FT_Library library;
    FT_Error error;
    FT_Face face;
    FT_Glyph ft_glyph;
    FT_GlyphSlot slot;
    FT_Bitmap ft_bitmap;

    FT_UInt glyph_index;
    FT_Int32 flags = 0;
    int ft_glyph_top = 0;
    int ft_glyph_left = 0;

    size_t missed = 0;

    assert( charcodes );

    width  = atlas->width;
    height = atlas->height;
    depth  = atlas->depth;

    if (!texture_font_get_face(font, &library, &face))
        return wcslen(charcodes);

    /* Load each glyph */
    for(size_t i=0; i<wcslen(charcodes); ++i ) {
        /* Check if charcode has been already loaded */
        if (font->getGlyph(charcodes[i]) != nullptr) {
            continue;
        }

        flags = 0;
        ft_glyph_top = 0;
        ft_glyph_left = 0;
        glyph_index = FT_Get_Char_Index( face, charcodes[i] );

        flags |= FT_LOAD_RENDER;
        flags |= FT_LOAD_FORCE_AUTOHINT;


        if( depth == 3 )
        {
            FT_Library_SetLcdFilter( library, FT_LCD_FILTER_LIGHT );
            flags |= FT_LOAD_TARGET_LCD;
//            if( font->filtering )
//            {
//                FT_Library_SetLcdFilterWeights( library, font->lcd_weights );
//            }
        }
        error = FT_Load_Glyph( face, glyph_index, flags );
        if( error )
        {
            fprintf( stderr, "FT_Error (line %d, code 0x%02x) : %s\n",
                     __LINE__, FT_Errors[error].code, FT_Errors[error].message );
            FT_Done_Face( face );
            FT_Done_FreeType( library );
            buf.reset();
            return wcslen(charcodes)-i;
        }

        // we will make stroke and shadow with shader instance of freetype library
        {
            slot            = face->glyph;
            ft_bitmap       = slot->bitmap;
            ft_glyph_top    = slot->bitmap_top;
            ft_glyph_left   = slot->bitmap_left;
        }


        // We want each glyph to be separated by at least one black pixel
        // (for example for shader used in demo-subpixel.c)
        w = ft_bitmap.width/depth + 1;
        h = ft_bitmap.rows + 1;
        Rect region = atlas->getRegin(w, h );
        if ( region.origin.x < 0 )
        {
            missed++;
            fprintf( stderr, "Texture atlas is full (line %d)\n",  __LINE__ );
            continue;
        }

        Rect realRegion(region);
        realRegion.size.width -= 1;
        realRegion.size.height -= 1;
        atlas->setRegin( realRegion, ft_bitmap.buffer, ft_bitmap.pitch );

        // Discard hinting to get advance
        FT_Load_Glyph( face, glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
        slot = face->glyph;

        Glyph::ptr glyph = Glyph::create();
        glyph->init(charcodes[i], ft_glyph_left, ft_glyph_top, slot->advance.x / HRESf, slot->advance.y / HRESf);

        TextureFrame::ptr frame = TextureFrame::create();
        frame->init(realRegion);
        frame->setTexture(atlas->getTexture());
        glyph->setTextureFrame(frame);

        font->addGlyph(glyph);
    }
    FT_Done_Face( face );
    FT_Done_FreeType( library );
    buf.reset();

    GenerateKerning(font);

    return missed;
}

}
