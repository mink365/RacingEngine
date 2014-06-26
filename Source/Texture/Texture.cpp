#include "Texture.h"

namespace re {

Texture::Texture()
{
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

int Texture::getHeight() const
{
    return height;
}


int Texture::getWidth() const
{
    return width;
}

std::string Texture::getPath() const
{
    return path;
}

void Texture::setPath(const std::string &value)
{
    this->path = value;

    int pos = value.find_last_of("/");
    int pos2 = value.find_last_of(".");
    this->name = value.substr(pos + 1, pos2 - pos - 1);
}

Uv Texture::getUv(const Vec2 &p) const
{
    return {p.x / this->width, p.y / this->height};
}

std::string Texture::getName() const
{
    return this->name;
}

}
