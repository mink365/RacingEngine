#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>
#include <string>
#include "Base/Singleton.h"

#include "Texture.h"
#include "TextureUtil.h"

namespace re {

class ImageLoader {
public:
    virtual void loadImage(Texture *texture, Image &image);
};

class TextureManager : public Singleton<TextureManager>
{
    friend class Singleton;
private:
    TextureManager() {};

public:
    void registerTexture(Texture::type& texture);
    void desposeTexture(Texture::constType& texture);

    bool containTexture(std::string name);
    Texture::type getTexture(std::string name);

    void loadTextures();

    void setImageLoader(ImageLoader *loader);

private:
    std::map<std::string, Texture::type> registeredTextures;

    TextureUtil textureUtil;

    ImageLoader *loader;
};

} // namespace re

#endif // TEXTUREMANAGER_H
