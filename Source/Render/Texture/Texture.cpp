#include "Texture.h"
#include "FileSystem/File.h"

namespace re {

Texture::Texture()
{
    this->width = 0;
    this->height = 0;

    this->glID = 0;
}

Texture::Texture(const int width, const int height, const int glID)
{
    this->width = width;
    this->height = height;
    this->glID = glID;
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
    return {p.x / this->width, (this->height - p.y) / this->height};
}

std::string Texture::getName() const
{
    return this->name;
}

}