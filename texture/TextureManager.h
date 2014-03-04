#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>
#include <string>
#include "Texture.h"
#include "TextureUtil.h"

class ImageLoader {
public:
    virtual void loadImage(Texture *texture, Image &image);
};

class TextureManager
{
public:
    static TextureManager &getInstance();

    void registerTexture(Texture &texture);
    void desposeTexture(const Texture &texture);

    bool containTexture(std::string name);
    Texture &getTexture(std::string name);

    void loadTextures();

    void setImageLoader(ImageLoader *loader);

private:
    TextureManager();

private:
    std::map<std::string, Texture *> registeredTextures;

    TextureUtil textureUtil;

    ImageLoader *loader;
};

#endif // TEXTUREMANAGER_H
