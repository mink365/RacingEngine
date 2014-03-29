#ifndef TEXTUREUTIL_H
#define TEXTUREUTIL_H

#include <image/Image.h>
#include <texture/Texture.h>

namespace re {

class TextureUtil
{
public:
    TextureUtil();

    void UploadTextureToHardware(Image &image, Texture &texture);
    void DeleteTextureFromHardware(Texture &texture);
};

} // namespace re

#endif // TEXTUREUTIL_H
