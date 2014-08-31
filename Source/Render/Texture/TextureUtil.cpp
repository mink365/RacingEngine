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

TextureFormat ImageFormatToPixelFormat(FORMAT format) {
    switch(format) {
    case FORMAT_R8:
        return TextureFormat::RED;
    case FORMAT_RG8:
        return TextureFormat::RG;
    case FORMAT_RGB8:
        return TextureFormat::RGB8;
    case FORMAT_RGBA8:
        return TextureFormat::RGBA8;
    case FORMAT_RGB16:
        return TextureFormat::RGB16;
    case FORMAT_RGBA16:
        return TextureFormat::RGBA16;
    case FORMAT_RGB32F:
        return TextureFormat::RGB32;
    case FORMAT_RGBA32F:
        return TextureFormat::RGBA32;
    case FORMAT_RGB565:
        return TextureFormat::RGB565;
    case FORMAT_RGBA4:
        return TextureFormat::RGBA4444;
    case FORMAT_D16:
        return TextureFormat::D16;
    case FORMAT_D24:
        return TextureFormat::D24;
    case FORMAT_DXT1:
        return TextureFormat::S3TC_DXT1;
    case FORMAT_DXT3:
        return TextureFormat::S3TC_DXT3;
    case FORMAT_DXT5:
        return TextureFormat::S3TC_DXT5;
    default:
        assert(false);
    }
}

void TexturePixelFormatToGL(TextureFormat format, GLint& internalFormat, GLenum& glformat, GLenum& gltype) {
    switch(format) {
    case TextureFormat::NONE:
    {
        assert(false);
    }
    case TextureFormat::RED:
    {
        internalFormat = GL_RED;
        glformat = GL_RED;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case TextureFormat::RG:
    {
        internalFormat = GL_RG;
        glformat = GL_RG;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case TextureFormat::RGB565:
    {
        internalFormat = GL_RGB;
        glformat = GL_RGB;
        gltype = GL_UNSIGNED_SHORT_5_6_5;

        break;
    }
    case TextureFormat::RGBA4444:
    {
        internalFormat = GL_RGBA;
        glformat = GL_RGBA;
        gltype = GL_UNSIGNED_SHORT_4_4_4_4;

        break;
    }
    case TextureFormat::RGBA5551:
    {
        internalFormat = GL_RGBA;
        glformat = GL_RGBA;
        gltype = GL_UNSIGNED_SHORT_5_5_5_1;

        break;
    }
    case TextureFormat::Alpha8:
    {
        internalFormat = GL_ALPHA;
        glformat = GL_ALPHA;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case TextureFormat::Luminance8:
    {
        internalFormat = GL_LUMINANCE;
        glformat = GL_LUMINANCE;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case TextureFormat::LuminanceAlpha8:
    {
        internalFormat = GL_LUMINANCE_ALPHA;
        glformat = GL_LUMINANCE_ALPHA;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case TextureFormat::RGB8:
    {
        internalFormat = GL_RGB;
        glformat = GL_RGB;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case TextureFormat::RGBA8:
    {
        internalFormat = GL_RGBA;
        glformat = GL_RGBA;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case TextureFormat::BGRA8:
    {
        internalFormat = GL_BGRA;
        glformat = GL_BGRA;
        gltype = GL_UNSIGNED_BYTE;

        break;
    }
    case TextureFormat::Alpha16:
    {
        internalFormat = GL_ALPHA;
        glformat = GL_ALPHA;
        gltype = GL_HALF_FLOAT_OES;

        break;
    }
    case TextureFormat::Luminance16:
    {
        internalFormat = GL_LUMINANCE;
        glformat = GL_LUMINANCE;
        gltype = GL_HALF_FLOAT_OES;

        break;
    }
    case TextureFormat::LuminanceAlpha16:
    {
        internalFormat = GL_LUMINANCE_ALPHA;
        glformat = GL_LUMINANCE_ALPHA;
        gltype = GL_HALF_FLOAT_OES;

        break;
    }
    case TextureFormat::RGB16:
    {
        internalFormat = GL_RGB;
        glformat = GL_RGB;
        gltype = GL_HALF_FLOAT_OES;

        break;
    }
    case TextureFormat::RGBA16:
    {
        internalFormat = GL_RGBA;
        glformat = GL_RGBA;
        gltype = GL_HALF_FLOAT_OES;

        break;
    }
    case TextureFormat::Alpha32:
    {
        internalFormat = GL_ALPHA;
        glformat = GL_ALPHA;
        gltype = GL_FLOAT;

        break;
    }
    case TextureFormat::Luminance32:
    {
        internalFormat = GL_LUMINANCE;
        glformat = GL_LUMINANCE;
        gltype = GL_FLOAT;

        break;
    }
    case TextureFormat::LuminanceAlpha32:
    {
        internalFormat = GL_LUMINANCE_ALPHA;
        glformat = GL_LUMINANCE_ALPHA;
        gltype = GL_FLOAT;

        break;
    }
    case TextureFormat::RGB32:
    {
        internalFormat = GL_RGB;
        glformat = GL_RGB;
        gltype = GL_FLOAT;

        break;
    }
    case TextureFormat::RGBA32:
    {
        internalFormat = GL_RGBA;
        glformat = GL_RGBA;
        gltype = GL_FLOAT;

        break;
    }
    case TextureFormat::D16:
    {
        internalFormat = GL_DEPTH_COMPONENT16;
        glformat = GL_LUMINANCE;
        gltype = GL_UNSIGNED_SHORT;

        break;
    }
    case TextureFormat::D24:
    {
        internalFormat = GL_DEPTH_COMPONENT24;
        glformat = GL_LUMINANCE;
        gltype = GL_UNSIGNED_INT;

        break;
    }
    case TextureFormat::PVRTC_RGB2:
    {
        internalFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
        glformat = 0;
        gltype = 0;

        break;
    }
    case TextureFormat::PVRTC_RGBA2:
    {
        internalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
        glformat = 0;
        gltype = 0;

        break;
    }
    case TextureFormat::PVRTC_RGB4:
    {
        internalFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
        glformat = 0;
        gltype = 0;

        break;
    }
    case TextureFormat::PVRTC_RGBA4:
    {
        internalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
        glformat = 0;
        gltype = 0;

        break;
    }
    case TextureFormat::PVRTC2_RGBA2:
    {
        internalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG;
        glformat = 0;
        gltype = 0;

        break;
    }
    case TextureFormat::PVRTC2_RGBA4:
    {
        internalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG;
        glformat = 0;
        gltype = 0;

        break;
    }
    case TextureFormat::ETC_RGB4:
    {
        internalFormat = GL_ETC1_RGB8_OES;
        glformat = 0;
        gltype = 0;

        break;
    }
    case TextureFormat::S3TC_DXT1:
    {
        internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        glformat = 0;
        gltype = 0;

        break;
    }
    case TextureFormat::S3TC_DXT3:
    {
        internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        glformat = 0;
        gltype = 0;

        break;
    }
    case TextureFormat::S3TC_DXT5:
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
    TextureFormat format = ImageFormatToPixelFormat(image.getFormat());
    texture.setFormat(format);

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
    TexturePixelFormatToGL(texture.getFormat(), internalFormat, srcFormat, srcType);

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
