#include "TextureManager.h"
#include "Util/StringUtil.h"
#include <vector>
#include <iostream>
#include "Image/Image.h"
#include "FileSystem/File.h"
#include "Base/Buffer.h"

namespace re {

void TextureManager::registerTexture(Texture::ptr& texture)
{
    if (this->registeredTextures.find(texture->getName()) == this->registeredTextures.end()) {
        this->registeredTextures[texture->getName()] = texture;
    } else {
        std::cout << "aready has key!: " << texture->getName() << std::endl;
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
    return this->registeredTextures[name];
}

void TextureManager::loadTextures()
{
    for (auto pair : this->registeredTextures) {
        Texture::ptr& texture = pair.second;

        Image image;

        this->loader->loadImage(texture.get(), image);

        textureUtil.UploadTextureToHardware(image, *texture);
    }
}

void TextureManager::setImageLoader(ImageLoader *loader)
{
    this->loader = loader;
}

void ImageLoader::loadImage(Texture *texture, Image &image)
{
    std::string ext = texture->getFile()->getExt();

    Buffer::ptr buf = texture->getFile()->read();

    if (ext == "png") {
        image.loadPNG(buf->getData(), buf->getSize());
    } else {
        image.loadJPEG(buf->getData(), buf->getSize());
    }
}

}
