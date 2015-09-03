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
    virtual void loadImage(Texture *texture, Pixmap &image);
};

class TextureManager : public Singleton<TextureManager>
{
    friend class Singleton;
private:
    TextureManager() {};

public:
    void registerTexture(Texture::ptr& texture);
    void desposeTexture(Texture::constPtr& texture);

    bool containTexture(std::string name);
    Texture::ptr getTexture(std::string name);

    void loadTextures();

    void setImageLoader(ImageLoader *loader);

private:
    std::map<std::string, Texture::ptr> registeredTextures;

    TextureUtil textureUtil;

    ImageLoader *loader;
};

} // namespace re

#endif // TEXTUREMANAGER_H
