#include "TextureUtil.h"

#include "opengl.h"
#include "Image/Image.h"
#include "Texture/Texture.h"
#include "GameHub.h"
#include "Renderer/Renderer.h"
#include "gl2extimg.h"

namespace re {

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

Texture::PixelFormat ImageFormatToPixelFormat(FORMAT format) {
    switch(format) {
    case FORMAT_R8:
        return Texture::PixelFormat::RED;
    case FORMAT_RG8:
        return Texture::PixelFormat::RG;
    case FORMAT_RGB8:
        return Texture::PixelFormat::RGB8;
    case FORMAT_RGBA8:
        return Texture::PixelFormat::RGBA8;
    case FORMAT_RGB16:
        return Texture::PixelFormat::RGB16;
    case FORMAT_RGBA16:
        return Texture::PixelFormat::RGBA16;
    case FORMAT_RGB32F:
        return Texture::PixelFormat::RGB32;
    case FORMAT_RGBA32F:
        return Texture::PixelFormat::RGBA32;
    case FORMAT_RGB565:
        return Texture::PixelFormat::RGB565;
    case FORMAT_RGBA4:
        return Texture::PixelFormat::RGBA4444;
    case FORMAT_D16:
        return Texture::PixelFormat::D16;
    case FORMAT_D24:
        return Texture::PixelFormat::D24;
    case FORMAT_DXT1:
        return Texture::PixelFormat::S3TC_DXT1;
    case FORMAT_DXT3:
        return Texture::PixelFormat::S3TC_DXT3;
    case FORMAT_DXT5:
        return Texture::PixelFormat::S3TC_DXT5;
    default:
        assert(false);
    }
}

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
    case Texture::PixelFormat::D16:
    {
        internalFormat = GL_DEPTH_COMPONENT16;
        glformat = GL_LUMINANCE;
        gltype = GL_UNSIGNED_SHORT;

        break;
    }
    case Texture::PixelFormat::D24:
    {
        internalFormat = GL_DEPTH_COMPONENT24;
        glformat = GL_LUMINANCE;
        gltype = GL_UNSIGNED_INT;

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
    case Texture::PixelFormat::S3TC_DXT1:
    {
        internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        glformat = 0;
        gltype = 0;

        break;
    }
    case Texture::PixelFormat::S3TC_DXT3:
    {
        internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        glformat = 0;
        gltype = 0;

        break;
    }
    case Texture::PixelFormat::S3TC_DXT5:
    {
        internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        glformat = 0;
        gltype = 0;

        break;
    }
    default:
        assert(false);
    }
}

GLenum TextureTargetTypeToGL(Texture::TargetType type) {
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
    Texture::PixelFormat format = ImageFormatToPixelFormat(image.getFormat());
    texture.setPixelFormat(format);

    UploadTextureToHardware(image.getPixels(), texture, index);
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

    GLenum srcFormat, srcType;
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
