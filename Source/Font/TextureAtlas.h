#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include <vector>

#include "platform.h"
#include "Math/BoundingVolume.h"

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
    Rect getRegin(float width, float height);
    void setRegin(const Rect& rect, const unsigned char *data, const Int stride);

    void clear();

private:
    Int fit(Int index, Int width, Int height) const;

    void merge();

private:
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
