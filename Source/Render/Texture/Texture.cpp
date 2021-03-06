#include "Texture.h"
#include "FileSystem/File.h"

namespace re {

Texture::Texture()
{
    this->width = 0;
    this->height = 0;

    this->glID = 0;

    this->target = TargetType::TEXTURE_2D;
    this->minFilter = FilterValue::LINEAR;
    this->magFilter = FilterValue::LINEAR;
    this->wrapU = WrapValue::CLAMP_TO_EDGE;
    this->wrapV = WrapValue::CLAMP_TO_EDGE;
}

Texture::Texture(const int width, const int height, const int glID)
{
    this->width = width;
    this->height = height;
    this->glID = glID;

    this->target = TargetType::TEXTURE_2D;
    this->minFilter = FilterValue::LINEAR;
    this->magFilter = FilterValue::LINEAR;
    this->wrapU = WrapValue::CLAMP_TO_EDGE;
    this->wrapV = WrapValue::CLAMP_TO_EDGE;
}

std::string Texture::getName() const
{
    return this->name;
}

GPU_ID Texture::getGlID() const
{
    return glID;
}

void Texture::setGlID(GPU_ID value)
{
    glID = value;
}

int Texture::getWidth() const
{
    return width;
}

int Texture::getHeight() const
{
    return height;
}

void Texture::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

FilePtr Texture::getFile() const
{
    return this->file;
}

void Texture::setFile(FilePtr &value)
{
    this->file = value;

    const std::string& path = value->getFullPath();

    int pos = path.find_last_of("/");
    int pos2 = path.find_last_of(".");
    this->name = path.substr(pos + 1, pos2 - pos - 1);
}

Uv Texture::getUv(const Vec2 &p) const
{
    return {p.x / this->width, p.y / this->height};
}

Texture::FilterValue Texture::getMinFilter() const
{
    return minFilter;
}

void Texture::setMinFilter(const FilterValue &value)
{
    minFilter = value;
}
Texture::FilterValue Texture::getMagFilter() const
{
    return magFilter;
}

void Texture::setMagFilter(const FilterValue &value)
{
    magFilter = value;
}
Texture::WrapValue Texture::getWrapU() const
{
    return wrapU;
}

void Texture::setWrapU(const WrapValue &value)
{
    wrapU = value;
}
Texture::WrapValue Texture::getWrapV() const
{
    return wrapV;
}

void Texture::setWrapV(const WrapValue &value)
{
    wrapV = value;
}

TextureFormat Texture::getFormat() const
{
    return format;
}

void Texture::setFormat(const TextureFormat &value)
{
    format = value;
}

Texture::TargetType Texture::getTarget() const
{
    return target;
}

void Texture::setTarget(const TargetType &value)
{
    target = value;
}


}
