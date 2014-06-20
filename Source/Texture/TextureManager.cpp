#include "TextureManager.h"
#include "Util/StringUtil.h"
#include <vector>
#include <iostream>
#include "Image/Image.h"

namespace re {

void TextureManager::registerTexture(Texture::type& texture)
{
    if (this->registeredTextures.find(texture->getName()) == this->registeredTextures.end()) {
        this->registeredTextures[texture->getName()] = texture;
    } else {
        std::cout << "aready has key!: " << texture->getName() << std::endl;
    }
}

void TextureManager::desposeTexture(Texture::constType& texture)
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

Texture::type TextureManager::getTexture(std::string name)
{
    return this->registeredTextures[name];
}

void TextureManager::loadTextures()
{
    for (auto pair : this->registeredTextures) {
        Texture::type& texture = pair.second;

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
    int pos = texture->getPath().find_last_of(".");
    std::string ext = texture->getPath().substr(pos + 1, texture->getPath().length());

    if (ext == "png") {
        image.loadPNG(texture->getPath().c_str());
    } else {
        image.loadJPEG(texture->getPath().c_str());
    }
}

}
