#ifndef TEXTURE_H
#define TEXTURE_H

#include "platform.h"
#include <string>
#include "Math/UV.h"

namespace re {

class Texture : public Shared<Texture>
{
public:
    Texture();
    Texture(const int width, const int height, const int glID);

    std::string getName() const;

    GPU_ID getGlID() const;
    void setGlID(GPU_ID value);

    int getWidth() const;
    int getHeight() const;

    ConstFilePtr getFile() const;
    void setFile(ConstFilePtr &value);

    Uv getUv(const Vec2& p) const;
private:
    ConstFilePtr file;
    std::string name;

    int width;
    int height;
    GPU_ID glID;
};

} // namespace re

#endif // TEXTURE_H
