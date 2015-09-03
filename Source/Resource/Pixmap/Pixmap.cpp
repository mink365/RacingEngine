#include "Pixmap.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jpeglib.h>
#include <png.h>

#include "platform.h"

namespace re {

void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length){
    fwrite(data, length, 1, (FILE *) png_get_io_ptr(png_ptr));
}

void user_flush_data(png_structp png_ptr){
    fflush((FILE *) png_get_io_ptr(png_ptr));
}

typedef struct
{
    unsigned char* data;
    int size;
    int offset;
} pngSource;

static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
    pngSource* isource = (pngSource*)png_get_io_ptr(png_ptr);

    if((int)(isource->offset + length) <= isource->size)
    {
        memcpy(data, isource->data+isource->offset, length);
        isource->offset += length;
    }
    else
    {
        png_error(png_ptr, "pngReaderCallback failed");
    }
}

void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length){
    fread(data, length, 1, (FILE *) png_get_io_ptr(png_ptr));
}

png_voidp malloc_fn(png_structp png_ptr, png_size_t size){
    return malloc(size);
}

void free_fn(png_structp png_ptr, png_voidp ptr){
    free(ptr);
}

Pixmap::Pixmap()
{
    pixels = NULL;
    width  = 0;
    height = 0;
    depth  = 0;
    nMipMaps = 0;
    arraySize = 0;
    format = FORMAT_NONE;

    nExtraData = 0;
    extraData = NULL;
}

Pixmap::~Pixmap()
{
    delete [] pixels;
    delete [] extraData;
}

bool Pixmap::loadJPEG(const char *fileName){
    jpeg_decompress_struct cinfo;
    jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    FILE *file;
    if ((file = fopen(fileName, "rb")) == NULL) return false;

    jpeg_stdio_src(&cinfo, file);

    bool result = this->loadJPEG(cinfo);

    fclose(file);

    return result;
}

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

static void my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

bool Pixmap::loadJPEG(void *pData, int datalen)
{
    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler.
     * Note that this struct must live as long as the main JPEG parameter
     * struct, to avoid dangling-pointer problems.
     */
    struct my_error_mgr jerr;

    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
        jpeg_destroy_decompress(&cinfo);
        return false;
    }

    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress( &cinfo );

    jpeg_mem_src( &cinfo, (unsigned char *) pData, datalen );

    bool result = this->loadJPEG(cinfo);

    return result;
}

bool Pixmap::loadJPEG(jpeg_decompress_struct &cinfo)
{
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    switch (cinfo.num_components){
        case 1:
            format = FORMAT_I8;
            break;
        case 3:
            format = FORMAT_RGB8;
            break;
        case 4:
            format = FORMAT_RGBA8;
            break;
    }
    width  = cinfo.output_width;
    height = cinfo.output_height;
    depth  = 1;
    nMipMaps = 1;
    arraySize = 1;

    pixels = new unsigned char[width * height * cinfo.num_components];
    unsigned char *curr_scanline = pixels;

    while (cinfo.output_scanline < cinfo.output_height){
        jpeg_read_scanlines(&cinfo, &curr_scanline, 1);
        curr_scanline += width * cinfo.num_components;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return true;
}

bool Pixmap::loadPNG(const char *fileName){
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    FILE *file;

    // open the PNG input file
    if ((file = fopen(fileName, "rb")) == NULL) return false;

    // first check the eight byte PNG signature
    png_byte pbSig[8];
    fread(pbSig, 1, 8, file);
    if (!png_check_sig(pbSig, 8)){
        fclose(file);
        return false;
    }

    // create the two png(-info) structures
    if ((png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, NULL, malloc_fn, free_fn)) == NULL){
        fclose(file);
        return false;
    }

    if ((info_ptr = png_create_info_struct(png_ptr)) == NULL){
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(file);
        return false;
    }

    // initialize the png structure
    png_set_read_fn(png_ptr, file, user_read_data);

    // 设定已经读过的字节数,fread有，而从data读时memcpy并没有造成指针的移动，即，没有读过
    png_set_sig_bytes(png_ptr, 8);

    this->loadPNG(png_ptr, info_ptr);

    fclose(file);

    return true;
}

#define CC_BREAK_IF(cond)            if(cond) return false;
bool Pixmap::loadPNG(void *pData, int datalen)
{
    // length of bytes to check if it is a valid png file
#define PNGSIGSIZE  8
    png_byte        header[PNGSIGSIZE]   = {0};
    png_structp     png_ptr     =   0;
    png_infop       info_ptr    = 0;

    // png header len is 8 bytes
    CC_BREAK_IF(datalen < PNGSIGSIZE);

    // check the data is png or not
    memcpy(header, pData, PNGSIGSIZE);
    CC_BREAK_IF(png_sig_cmp(header, 0, PNGSIGSIZE));

    // init png_struct
    png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, NULL, malloc_fn, free_fn);
    CC_BREAK_IF(! png_ptr);

    // init png_info
    info_ptr = png_create_info_struct(png_ptr);
    CC_BREAK_IF(!info_ptr);

//#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA && CC_TARGET_PLATFORM != CC_PLATFORM_NACL)
//    CC_BREAK_IF(setjmp(png_jmpbuf(png_ptr)));
//#endif

    // set the read call back function
    pngSource imageSource;
    imageSource.data    = (unsigned char*)pData;
    imageSource.size    = datalen;
    imageSource.offset  = 0;

    png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

    bool result = this->loadPNG(png_ptr, info_ptr);

    return result;
}

// premultiply alpha, or the effect will wrong when want to use other pixel format in CCTexture2D,
// such as RGB888, RGB5A1
#define CC_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
    ((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
    ((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
    ((unsigned)(unsigned char)(va) << 24))

bool Pixmap::loadPNG(png_structp &png_ptr, png_infop &info_ptr)
{
    // read all PNG info up to image data
    png_read_info(png_ptr, info_ptr);

    // get width, height, bit-depth and color-type
    png_uint_32 w, h;
    int bitDepth, colorType;
    png_get_IHDR(png_ptr, info_ptr, &w, &h, &bitDepth, &colorType, NULL, NULL, NULL);

    width = w;
    height = h;
    depth = 1;
    nMipMaps = 1;
    arraySize = 1;

    int nChannels = png_get_channels(png_ptr, info_ptr);
    switch (nChannels){
        case 1:
            format = (bitDepth > 8)? FORMAT_I16 : FORMAT_I8;
            break;
        case 2:
            format = (bitDepth > 8)? FORMAT_IA16 : FORMAT_IA8;
            break;
        case 3:
            format = (bitDepth > 8)? FORMAT_RGB16 : FORMAT_RGB8;
            break;
        case 4:
            format = (bitDepth > 8)? FORMAT_RGBA16 : FORMAT_RGBA8;
            break;
    }

    uint rowSize = width * nChannels * bitDepth / 8;

    // now we can allocate memory to store the image
    pixels = new ubyte[rowSize * height];

    // set the individual row-pointers to point at the correct offsets
    png_byte **ppbRowPointers = new png_bytep[height];
    for (int i = 0; i < height; i++)
        ppbRowPointers[i] = pixels + i * rowSize;

    // now we can go ahead and just read the whole image
    png_read_image(png_ptr, ppbRowPointers);

    // read the additional chunks in the PNG file (not really needed)
    png_read_end(png_ptr, NULL);

    // 如果有alpha，要预乘。否则会出现白色的区段(PAD隧道的黑色阴影图)
    if (nChannels == 4)
    {
        unsigned int *tmp = (unsigned int *)pixels;
        for(unsigned short i = 0; i < height; i++)
        {
            for(unsigned int j = 0; j < rowSize; j += 4)
            {
                *tmp++ = CC_RGB_PREMULTIPLY_ALPHA( ppbRowPointers[i][j], ppbRowPointers[i][j + 1],
                                                  ppbRowPointers[i][j + 2], ppbRowPointers[i][j + 3] );
            }
        }
    }

    delete [] ppbRowPointers;

    if (colorType == PNG_COLOR_TYPE_PALETTE){
        png_colorp palette;
        int num_palette;
        png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);

        ubyte *newPixels = new ubyte[width * height * 3];
        if (bitDepth == 4){
            for (uint i = 0; i < rowSize * height; i++){
                uint i0 = pixels[i] >> 4;
                uint i1 = pixels[i] & 0xF;
                newPixels[6 * i    ] = palette[i0].red;
                newPixels[6 * i + 1] = palette[i0].green;
                newPixels[6 * i + 2] = palette[i0].blue;
                newPixels[6 * i + 3] = palette[i1].red;
                newPixels[6 * i + 4] = palette[i1].green;
                newPixels[6 * i + 5] = palette[i1].blue;
            }
        } else {
            for (uint i = 0; i < rowSize * height; i++){
                newPixels[3 * i    ] = palette[pixels[i]].red;
                newPixels[3 * i + 1] = palette[pixels[i]].green;
                newPixels[3 * i + 2] = palette[pixels[i]].blue;
            }
        }
        format = FORMAT_RGB8;

        delete [] pixels;
        pixels = newPixels;
    }

    if (bitDepth == 16){
        // Fix endian
        int size = width * height * nChannels * sizeof(ushort);
        for (int i = 0; i < size; i += 2){
            ubyte tmp = pixels[i];
            pixels[i] = pixels[i + 1];
            pixels[i + 1] = tmp;
        }
    }

    // and we're done
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return true;
}

unsigned char *Pixmap::getPixels() const
{
    return pixels;
}

int Pixmap::getDepth() const
{
    return depth;
}

int Pixmap::getWidth() const
{
    return width;
}

int Pixmap::getHeight() const
{
    return height;
}

}
