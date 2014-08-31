#include "TextureUtil.h"

#include "opengl.h"
#include "Image/Image.h"
#include "Texture/Texture.h"
#include "GameHub.h"
#include "Renderer/Renderer.h"
#include "gl2extimg.h"

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

static const GLenum TargetTypes[] {
    GL_TEXTURE_1D,
    GL_TEXTURE_2D,
    GL_TEXTURE_3D,
    GL_TEXTURE_CUBE_MAP,
};

void TexturePixelFormatToGL(Texture::PixelFormat format, GLint& internalFormat, GLenum& glformat, GLenum& gltype) {
    switch(format) {
    case Texture::PixelFormat::NONE:
    {
        assert(false);
    }
    case Texture::PixelFormat::RED:
    {
        internalFormat = GL_RED;
        glformat = GL_RED;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case Texture::PixelFormat::RG:
    {
        internalFormat = GL_RG;
        glformat = GL_RG;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case Texture::PixelFormat::RGB565:
    {
        internalFormat = GL_RGB;
        glformat = GL_RGB;
        gltype = GL_UNSIGNED_SHORT_5_6_5;

        break;
    }
    case Texture::PixelFormat::RGBA4444:
    {
        internalFormat = GL_RGBA;
        glformat = GL_RGBA;
        gltype = GL_UNSIGNED_SHORT_4_4_4_4;

        break;
    }
    case Texture::PixelFormat::RGBA5551:
    {
        internalFormat = GL_RGBA;
        glformat = GL_RGBA;
        gltype = GL_UNSIGNED_SHORT_5_5_5_1;

        break;
    }
    case Texture::PixelFormat::Alpha8:
    {
        internalFormat = GL_ALPHA;
        glformat = GL_ALPHA;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case Texture::PixelFormat::Luminance8:
    {
        internalFormat = GL_LUMINANCE;
        glformat = GL_LUMINANCE;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case Texture::PixelFormat::LuminanceAlpha8:
    {
        internalFormat = GL_LUMINANCE_ALPHA;
        glformat = GL_LUMINANCE_ALPHA;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case Texture::PixelFormat::RGB8:
    {
        internalFormat = GL_RGB;
        glformat = GL_RGB;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case Texture::PixelFormat::RGBA8:
    {
        internalFormat = GL_RGBA;
        glformat = GL_RGBA;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case Texture::PixelFormat::BGRA8:
    {
        internalFormat = GL_BGRA;
        glformat = GL_BGRA;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case Texture::PixelFormat::Alpha16:
    {
        internalFormat = GL_ALPHA;
        glformat = GL_ALPHA;
        gltype = GL_HALF_FLOAT_OES;

        break;
    }
    case Texture::PixelFormat::Luminance16:
    {
        internalFormat = GL_LUMINANCE;
        glformat = GL_LUMINANCE;
        gltype = GL_HALF_FLOAT_OES;

        break;
    }
    case Texture::PixelFormat::LuminanceAlpha16:
    {
        internalFormat = GL_LUMINANCE_ALPHA;
        glformat = GL_LUMINANCE_ALPHA;
        gltype = GL_HALF_FLOAT_OES;

        break;
    }
    case Texture::PixelFormat::RGB16:
    {
        internalFormat = GL_RGB;
        glformat = GL_RGB;
        gltype = GL_HALF_FLOAT_OES;

        break;
    }
    case Texture::PixelFormat::RGBA16:
    {
        internalFormat = GL_RGBA;
        glformat = GL_RGBA;
        gltype = GL_HALF_FLOAT_OES;

        break;
    }
    case Texture::PixelFormat::Alpha32:
    {
        internalFormat = GL_ALPHA;
        glformat = GL_ALPHA;
        gltype = GL_FLOAT;

        break;
    }
    case Texture::PixelFormat::Luminance32:
    {
        internalFormat = GL_LUMINANCE;
        glformat = GL_LUMINANCE;
        gltype = GL_FLOAT;

        break;
    }
    case Texture::PixelFormat::LuminanceAlpha32:
    {
        internalFormat = GL_LUMINANCE_ALPHA;
        glformat = GL_LUMINANCE_ALPHA;
        gltype = GL_FLOAT;

        break;
    }
    case Texture::PixelFormat::RGB32:
    {
        internalFormat = GL_RGB;
        glformat = GL_RGB;
        gltype = GL_FLOAT;

        break;
    }
    case Texture::PixelFormat::RGBA32:
    {
        internalFormat = GL_RGBA;
        glformat = GL_RGBA;
        gltype = GL_FLOAT;

        break;
    }
    case Texture::PixelFormat::PVRTC2:
    {
        internalFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
        glformat = 0;
        gltype = 0;

        break;
    }
    case Texture::PixelFormat::PVRTC2A:
    {
        internalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
        glformat = 0;
        gltype = 0;

        break;
    }
    case Texture::PixelFormat::PVRTC4:
    {
        internalFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
        glformat = 0;
        gltype = 0;

        break;
    }
    case Texture::PixelFormat::PVRTC4A:
    {
        internalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
        glformat = 0;
        gltype = 0;

        break;
    }
    case Texture::PixelFormat::PVRTCII2:
    {
        internalFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
        glformat = 0;
        gltype = 0;

        break;
    }
    case Texture::PixelFormat::PVRTCII4:
    {
        internalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG;
        glformat = 0;
        gltype = 0;

        break;
    }
    case Texture::PixelFormat::ETC:
    {
        internalFormat = GL_ETC1_RGB8_OES;
        glformat = 0;
        gltype = 0;

        break;
    }
    default:
        assert(false);
    }
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

    GLenum srcFormat;
    GLenum srcType;
    GLint internalFormat;
    TexturePixelFormatToGL(texture.getPixelFormat(), internalFormat, srcFormat, srcType);

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
