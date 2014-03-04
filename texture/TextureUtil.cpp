#include "TextureUtil.h"

#include "opengl.h"

static const GLint internalFormats[] = {
    0,
    // Unsigned formats
    0, // GL_INTENSITY8,
    0, // GL_LUMINANCE8_ALPHA8,
    GL_RGB, // GL_RGB8,
    GL_RGBA, // GL_RGBA8,

    0, // GL_INTENSITY16,
    0, // GL_LUMINANCE16_ALPHA16,
    0, // GL_RGB16,
    GL_RGBA16, // GL_RGBA16,

    // Signed formats
    0,
    0,
    0,
    0,

    0,
    0,
    0,
    0,

    // Float formats
    0, // GL_INTENSITY_FLOAT16_ATI,
    0, // GL_LUMINANCE_ALPHA_FLOAT16_ATI,
    0, // GL_RGB_FLOAT16_ATI,
    0, // GL_RGBA_FLOAT16_ATI,

    0, // GL_INTENSITY_FLOAT32_ATI,
    0, // GL_LUMINANCE_ALPHA_FLOAT32_ATI,
    0, // GL_RGB_FLOAT32_ATI,
    0, // GL_RGBA_FLOAT32_ATI,

    // Signed integer formats
    0, // GL_INTENSITY16I_EXT,
    0, // GL_LUMINANCE_ALPHA16I_EXT,
    0, // GL_RGB16I_EXT,
    0, // GL_RGBA16I_EXT,

    0, // GL_INTENSITY32I_EXT,
    0, // GL_LUMINANCE_ALPHA32I_EXT,
    0, // GL_RGB32I_EXT,
    0, // GL_RGBA32I_EXT,

    // Unsigned integer formats
    0, // GL_INTENSITY16UI_EXT,
    0, // GL_LUMINANCE_ALPHA16UI_EXT,
    0, // GL_RGB16UI_EXT,
    0, // GL_RGBA16UI_EXT,

    0, // GL_INTENSITY32UI_EXT,
    0, // GL_LUMINANCE_ALPHA32UI_EXT,
    0, // GL_RGB32UI_EXT,
    0, // GL_RGBA32UI_EXT,

    // Packed formats
    0, // RGBE8 not directly supported
    0, // GL_RGB9_E5,
    0, // GL_R11F_G11F_B10F,
    GL_RGB, // GL_RGB5,
    GL_RGBA, // GL_RGBA4,
    0, // GL_RGB10_A2,

    // Depth formats
    GL_DEPTH_COMPONENT16,
    0, // GL_DEPTH_COMPONENT24,
    0, // GL_DEPTH24_STENCIL8_EXT,
    0, // GL_DEPTH_COMPONENT32F,

    // Compressed formats
    GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
    0, // GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
    0, // GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
    0, // ATI1N not yet supported
    0, // GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI,
};

static const GLenum srcFormats[] = { 0, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA };

static const GLenum srcTypes[] = {
    0,
    // Unsigned formats
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,

    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_SHORT,

    // Signed formats
    0,
    0,
    0,
    0,

    0,
    0,
    0,
    0,

    // Float formats
    0,//GL_HALF_FLOAT_ARB,
    0,//GL_HALF_FLOAT_ARB,
    0,//GL_HALF_FLOAT_ARB,
    0,//GL_HALF_FLOAT_ARB,

    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,

    // Signed integer formats
    0,
    0,
    0,
    0,

    0,
    0,
    0,
    0,

    // Unsigned integer formats
    0,
    0,
    0,
    0,

    0,
    0,
    0,
    0,

    // Packed formats
    0, // RGBE8 not directly supported
    0, // RGBE9E5 not supported
    0, // RG11B10F not supported
    GL_UNSIGNED_SHORT_5_6_5,
    GL_UNSIGNED_SHORT_4_4_4_4, // GL_UNSIGNED_SHORT_4_4_4_4_REV,
    0, // GL_UNSIGNED_INT_2_10_10_10_REV,

    // Depth formats
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT,
    0, // GL_UNSIGNED_INT_24_8_EXT,
    0, // D32F not supported

    // Compressed formats
    0,
    0,
    0,
    0,
    0,
};

TextureUtil::TextureUtil()
{
}

void TextureUtil::UploadTextureToHardware(Image &image, Texture &texture)
{
    GLuint id = 0;
    glGenTextures(1, &id);

    glBindTexture( GL_TEXTURE_2D, id );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    FORMAT format = image.getFormat();
    GLenum srcFormat = srcFormats[getChannelCount(format)];
    GLenum srcType = srcTypes[format];
    GLint internalFormat = internalFormats[format];

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.getWidth(), image.getHeight(), 0, srcFormat, srcType, image.getPixels());

    texture.setGlID(id);
}

void TextureUtil::DeleteTextureFromHardware(Texture &texture)
{
    GLuint id = texture.getGlID();
    glDeleteTextures(1, &id);
}
