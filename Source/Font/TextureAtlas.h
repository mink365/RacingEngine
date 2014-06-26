#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include <vector>

#include "platform.h"
#include "Math/BoundingVolume.h"
#include "Texture/Texture.h"

namespace re {

struct AtlasNode {
    Int x;
    Int y;

    Int width;
};

class TextureAtlas : public Shared<TextureAtlas>
{
    friend class FreeTypeUtil;

public:
    TextureAtlas();
    ~TextureAtlas();

public:
    void init(Int width, Int height, Int depth);

    Rect getRegin(float width, float height);
    void setRegin(const Rect& rect, const unsigned char *data, const Int stride);

    void clear();

    Texture::ptr getTexture();

    void upload();

private:
    Int fit(Int index, Int width, Int height) const;

    void merge();

private:
    Texture::ptr texture;

    std::vector<AtlasNode*> nodes;

    /**
     * Atlas data
     */
    unsigned char * data;

    /**
     * @brief depth
     * pixel depth, in byte
     */
    Int depth;

    Int width;
    Int height;

    Int used;
};

}

#endif // TEXTUREATLAS_H
