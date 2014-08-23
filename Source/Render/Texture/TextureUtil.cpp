#include "TextureUtil.h"

#include "opengl.h"
#include "Image/Image.h"
#include "Texture/Texture.h"
#include "GameHub.h"
#include "Renderer/Renderer.h"

namespace re {

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

static const GLint FilterValues[] {
    0,
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_LINEAR,
};

static const GLint WrapValues[] {
    0,
    GL_CLAMP_TO_EDGE,
    GL_MIRRORED_REPEAT,
    GL_REPEAT,
};

static const GLint InternalFormats[] {
    0,
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_RGBA16,
};

static const GLenum PixelFormats[] {
    0,
    GL_RED,
    GL_RG,
    GL_ALPHA,
    GL_LUMINANCE,
    GL_LUMINANCE_ALPHA,
    GL_RGB,
    GL_RGBA,
};

static const GLenum DataTypes[] {
    0,
    GL_UNSIGNED_BYTE,
    GL_BYTE,
    GL_UNSIGNED_SHORT_5_6_5,
    GL_SHORT,
    GL_UNSIGNED_INT,
    GL_INT,
    GL_FLOAT,
};

static const GLenum TargetTypes[] {
    GL_TEXTURE_2D,
    GL_TEXTURE_CUBE_MAP,
};

const GLenum TextureInternalFormatToGL(Texture::InternalFormat format) {
    return InternalFormats[(int)format];
}

const GLenum TexturePixelFormatToGL(Texture::PixelFormat format) {
    return PixelFormats[(int)format];
}

const GLenum TextureDataTypeToGL(Texture::DataType type) {
    return DataTypes[(int)type];
}

const GLenum TextureTargetTypeToGL(Texture::TargetType type) {
    return TargetTypes[(int)type];
}

Renderer& GetRenderer() {
    return GameHub::getInstance().GetRenderer();
}

TextureUtil::TextureUtil()
{
}

void TextureUtil::UploadTextureToHardware(Image &image, Texture &texture, int index)
{
    if( !texture.getGlID() )
    {
        GLuint id = 0;

        glGenTextures( 1, &id );

        texture.setGlID(id);
    }

    GLenum target = TextureTargetTypeToGL(texture.getTarget());

    GetRenderer().bindTexture(0, true, texture);

    GLint wrapS = WrapValues[(int)texture.getWrapU()];
    GLint wrapT = WrapValues[(int)texture.getWrapV()];
    GLint minFilter = FilterValues[(int)texture.getMinFilter()];
    GLint magFilter = FilterValues[(int)texture.getMagFilter()];

    glTexParameteri( target, GL_TEXTURE_WRAP_S, wrapS );
    glTexParameteri( target, GL_TEXTURE_WRAP_T, wrapT );
    glTexParameteri( target, GL_TEXTURE_MIN_FILTER, minFilter );
    glTexParameteri( target, GL_TEXTURE_MAG_FILTER, magFilter );

    FORMAT format = image.getFormat();
    GLenum srcFormat = srcFormats[getChannelCount(format)];
    GLenum srcType = srcTypes[format];
    GLint internalFormat = internalFormats[format];

    unsigned char* data = image.getPixels();

    if (target == GL_TEXTURE_CUBE_MAP) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+index, 0, internalFormat, image.getWidth(), image.getHeight(), 0, srcFormat, srcType, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.getWidth(), image.getHeight(), 0, srcFormat, srcType, data);
    }
}

void TextureUtil::UploadTextureToHardware(unsigned char *data, Texture &texture, int index)
{
    if( !texture.getGlID() )
    {
        GLuint id = 0;

        glGenTextures( 1, &id );

        texture.setGlID(id);
    }
    GLenum target = TextureTargetTypeToGL(texture.getTarget());

    GetRenderer().bindTexture(0, true, texture);

    GLint wrapS = WrapValues[(int)texture.getWrapU()];
    GLint wrapT = WrapValues[(int)texture.getWrapV()];
    GLint minFilter = FilterValues[(int)texture.getMinFilter()];
    GLint magFilter = FilterValues[(int)texture.getMagFilter()];

    glTexParameteri( target, GL_TEXTURE_WRAP_S, wrapS );
    glTexParameteri( target, GL_TEXTURE_WRAP_T, wrapT );
    glTexParameteri( target, GL_TEXTURE_MIN_FILTER, minFilter );
    glTexParameteri( target, GL_TEXTURE_MAG_FILTER, magFilter );

    GLenum srcFormat = PixelFormats[(int)texture.getPixelFormat()];
    GLenum srcType = DataTypes[(int)texture.getDataType()];
    GLint internalFormat = InternalFormats[(int)texture.getInternalFormat()];

    if (target == GL_TEXTURE_CUBE_MAP) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+index, 0, internalFormat, texture.getWidth(), texture.getHeight(), 0, srcFormat, srcType, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture.getWidth(), texture.getHeight(), 0, srcFormat, srcType, data);
    }
}

void TextureUtil::DeleteTextureFromHardware(Texture &texture)
{
    GLuint id = texture.getGlID();
    glDeleteTextures(1, &id);
}

}
