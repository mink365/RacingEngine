#ifndef TEXTUREUTIL_H
#define TEXTUREUTIL_H

namespace re {

class Image;
class Texture;

class TextureUtil
{
public:
    TextureUtil();

    static void UploadTextureToHardware(Image &image, Texture &texture, int index=0);
    static void UploadTextureToHardware(unsigned char *data, Texture &texture, int index=0);
    static void DeleteTextureFromHardware(Texture &texture);
};

} // namespace re

#endif // TEXTUREUTIL_H
