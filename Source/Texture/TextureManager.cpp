#include "TextureManager.h"
#include "Util/StringUtil.h"
#include <vector>
#include <iostream>
#include "Image/Image.h"

namespace re {

TextureManager::TextureManager()
{
}

TextureManager &TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

void TextureManager::registerTexture(Texture &texture)
{
    if (this->registeredTextures.find(texture.getName()) == this->registeredTextures.end()) {
        this->registeredTextures[texture.getName()] = &texture;
    } else {
        std::cout << "aready has key!: " << texture.getName() << std::endl;
    }
}

void TextureManager::desposeTexture(const Texture &texture)
{
    this->registeredTextures.erase(texture.getName());
}

bool TextureManager::containTexture(std::string name)
{
    if (this->registeredTextures.find(name) != this->registeredTextures.end()) {
        return true;
    }

    return false;
}

Texture &TextureManager::getTexture(std::string name)
{
    return *(this->registeredTextures[name]);
}

void TextureManager::loadTextures()
{
    std::map<std::string, Texture *>::iterator iter;
    for (iter = this->registeredTextures.begin(); iter != this->registeredTextures.end(); ++iter) {
        Texture *texture = iter->second;

        Image image;

        this->loader->loadImage(texture, image);

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
