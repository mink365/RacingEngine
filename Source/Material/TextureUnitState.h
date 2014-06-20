#ifndef TEXTUREUNITSTATE_H
#define TEXTUREUNITSTATE_H

#include <vector>
#include "Texture/Texture.h"

namespace re {

class TextureUnitState
{
public:
    TextureUnitState();

    float getRotation() const;
    void setRotation(float value);

    float getOffsetU() const;
    void setOffsetU(float value);

    float getOffsetV() const;
    void setOffsetV(float value);

    float getScaleU() const;
    void setScaleU(float value);

    float getScaleV() const;
    void setScaleV(float value);

    void setUVstate(float offsetU, float offsetV, float scaleU, float scaleV, float rotation);

    Texture::type getActivityTexture();
    void addTextureFrame(Texture::type texture);

private:
    std::vector<Texture::type> frames;

    int currentIndex;

    float offsetU;
    float offsetV;
    float scaleU;
    float scaleV;
    float rotation;
};

} // namespace re

#endif // TEXTUREUNITSTATE_H
