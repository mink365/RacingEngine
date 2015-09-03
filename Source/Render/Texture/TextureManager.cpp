#include "TextureManager.h"
#include <vector>
#include <iostream>
#include "Pixmap/Pixmap.h"
#include "FileSystem/File.h"
#include "Base/Memory/Buffer.h"

namespace re {

void TextureManager::registerTexture(Texture::ptr& texture)
{
    if (this->registeredTextures.find(texture->getName()) == this->registeredTextures.end()) {
        this->registeredTextures[texture->getName()] = texture;
    } else {
        LogError("aready has key: {}", texture->getName());
    }
}

void TextureManager::desposeTexture(Texture::constPtr& texture)
{
    this->registeredTextures.erase(texture->getName());
}

bool TextureManager::containTexture(std::string name)
{
    if (this->registeredTextures.find(name) != this->registeredTextures.end()) {
        return true;
    }

    return false;
}

Texture::ptr TextureManager::getTexture(std::string name)
{
    if (this->containTexture(name)) {
        return this->registeredTextures[name];
    }

    return nullptr;
}

void TextureManager::loadTextures()
{
    for (auto pair : this->registeredTextures) {
        Texture::ptr& texture = pair.second;

        if (texture->getGlID() != 0) {
            continue;
        }

        Pixmap image;

        this->loader->loadImage(texture.get(), image);
        texture->setSize(image.getWidth(), image.getHeight());

        textureUtil.UploadTextureToHardware(image, *texture);
    }
}

void TextureManager::setImageLoader(ImageLoader *loader)
{
    this->loader = loader;
}

void ImageLoader::loadImage(Texture *texture, Pixmap &image)
{
    std::string ext = texture->getFile()->getExt();

    ByteBufferPtr buf = texture->getFile()->read();

    if (ext == "png") {
        image.loadPNG(buf->getData(), buf->getSize());
    } else {
        image.loadJPEG(buf->getData(), buf->getSize());
    }
}

}
