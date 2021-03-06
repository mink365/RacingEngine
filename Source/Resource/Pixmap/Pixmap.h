#ifndef RE_PIXMAP_H
#define RE_PIXMAP_H

#include <stdio.h>
#include <stdlib.h>

#include <jpeglib.h>
#include <png.h>

namespace re {

typedef unsigned char ubyte;

enum FORMAT {
    FORMAT_NONE     = 0,

    // Unsigned formats
    FORMAT_R8       = 1,
    FORMAT_RG8      = 2,
    FORMAT_RGB8     = 3,
    FORMAT_RGBA8    = 4,

    FORMAT_R16      = 5,
    FORMAT_RG16     = 6,
    FORMAT_RGB16    = 7,
    FORMAT_RGBA16   = 8,

    // Signed formats
    FORMAT_R8S      = 9,
    FORMAT_RG8S     = 10,
    FORMAT_RGB8S    = 11,
    FORMAT_RGBA8S   = 12,

    FORMAT_R16S     = 13,
    FORMAT_RG16S    = 14,
    FORMAT_RGB16S   = 15,
    FORMAT_RGBA16S  = 16,

    // Float formats
    FORMAT_R16F     = 17,
    FORMAT_RG16F    = 18,
    FORMAT_RGB16F   = 19,
    FORMAT_RGBA16F  = 20,

    FORMAT_R32F     = 21,
    FORMAT_RG32F    = 22,
    FORMAT_RGB32F   = 23,
    FORMAT_RGBA32F  = 24,

    // Signed integer formats
    FORMAT_R16I     = 25,
    FORMAT_RG16I    = 26,
    FORMAT_RGB16I   = 27,
    FORMAT_RGBA16I  = 28,

    FORMAT_R32I     = 29,
    FORMAT_RG32I    = 30,
    FORMAT_RGB32I   = 31,
    FORMAT_RGBA32I  = 32,

    // Unsigned integer formats
    FORMAT_R16UI    = 33,
    FORMAT_RG16UI   = 34,
    FORMAT_RGB16UI  = 35,
    FORMAT_RGBA16UI = 36,

    FORMAT_R32UI    = 37,
    FORMAT_RG32UI   = 38,
    FORMAT_RGB32UI  = 39,
    FORMAT_RGBA32UI = 40,

    // Packed formats
    FORMAT_RGBE8    = 41,
    FORMAT_RGB9E5   = 42,
    FORMAT_RG11B10F = 43,
    FORMAT_RGB565   = 44,
    FORMAT_RGBA4    = 45,
    FORMAT_RGB10A2  = 46,

    // Depth formats
    FORMAT_D16      = 47,
    FORMAT_D24      = 48,
    FORMAT_D24S8    = 49,
    FORMAT_D32F     = 50,

    // Compressed formats
    FORMAT_DXT1     = 51,
    FORMAT_DXT3     = 52,
    FORMAT_DXT5     = 53,
    FORMAT_ATI1N    = 54,
    FORMAT_ATI2N    = 55
};

#define FORMAT_I8    FORMAT_R8
#define FORMAT_IA8   FORMAT_RG8
#define FORMAT_I16   FORMAT_R16
#define FORMAT_IA16  FORMAT_RG16
#define FORMAT_I16F  FORMAT_R16F
#define FORMAT_IA16F FORMAT_RG16F
#define FORMAT_I32F  FORMAT_R32F
#define FORMAT_IA32F FORMAT_RG32F

// Compatibility with old demos
#define FORMAT_DEPTH16 FORMAT_D16
#define FORMAT_DEPTH24 FORMAT_D24

inline int getChannelCount(const FORMAT format){
    static const int chCount[] = {
        0,
        1, 2, 3, 4,       //  8-bit unsigned
        1, 2, 3, 4,       // 16-bit unsigned
        1, 2, 3, 4,       //  8-bit signed
        1, 2, 3, 4,       // 16-bit signed
        1, 2, 3, 4,       // 16-bit float
        1, 2, 3, 4,       // 32-bit float
        1, 2, 3, 4,       // 16-bit unsigned integer
        1, 2, 3, 4,       // 32-bit unsigned integer
        1, 2, 3, 4,       // 16-bit signed integer
        1, 2, 3, 4,       // 32-bit signed integer
        3, 3, 3, 3, 4, 4, // Packed
        1, 1, 2, 1,       // Depth
        3, 4, 4, 1, 2,    // Compressed
    };
    return chCount[format];
}

// Accepts only plain formats
inline int getBytesPerChannel(const FORMAT format){
    static const int bytesPC[] = {
        1, //  8-bit unsigned
        2, // 16-bit unsigned
        1, //  8-bit signed
        2, // 16-bit signed
        2, // 16-bit float
        4, // 32-bit float
        2, // 16-bit unsigned integer
        4, // 32-bit unsigned integer
        2, // 16-bit signed integer
        4, // 32-bit signed integer
    };

    return bytesPC[(format - 1) >> 2];
}

// Does not accept compressed formats
inline int getBytesPerPixel(const FORMAT format){
    static const int bytesPP[] = {
        0,
        1, 2, 3, 4,       //  8-bit unsigned
        2, 4, 6, 8,       // 16-bit unsigned
        1, 2, 3, 4,       //  8-bit signed
        2, 4, 6, 8,       // 16-bit signed
        2, 4, 6, 8,       // 16-bit float
        4, 8, 12, 16,     // 32-bit float
        2, 4, 6, 8,       // 16-bit unsigned integer
        4, 8, 12, 16,     // 32-bit unsigned integer
        2, 4, 6, 8,       // 16-bit signed integer
        4, 8, 12, 16,     // 32-bit signed integer
        4, 4, 4, 2, 2, 4, // Packed
        2, 4, 4, 4,       // Depth
    };
    return bytesPP[format];
}

// Accepts only compressed formats
inline int getBytesPerBlock(const FORMAT format){
    return (format == FORMAT_DXT1 || format == FORMAT_ATI1N)? 8 : 16;
}

class Pixmap
{
public:
    Pixmap();
    ~Pixmap();

    bool loadJPEG(const char *fileName);
    bool loadJPEG(void * pData, int datalen);
    bool loadJPEG(jpeg_decompress_struct &cinfo);

    bool loadPNG(const char *fileName);
    bool loadPNG(void * pData, int datalen);
    bool loadPNG(png_structp &png_ptr, png_infop &info_ptr);

    int getHeight() const;

    int getWidth() const;

    int getDepth() const;

    FORMAT getFormat() const { return format; }
    void setFormat(const FORMAT form){ format = form; }

    unsigned char *getPixels() const;

private:
    unsigned char *pixels;
    int width, height, depth;
    int nMipMaps;
    int arraySize;
    FORMAT format;

    int nExtraData;
    unsigned char *extraData;
};

} // namespace re

#endif // RE_PIXMAP_H
