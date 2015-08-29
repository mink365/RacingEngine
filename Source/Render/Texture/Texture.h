#ifndef TEXTURE_H
#define TEXTURE_H

#include "platform.h"
#include <string>
#include "Math/UV.h"
#include "Math/Vector.h"

namespace re {

class File;
using FilePtr = SharedPtr<File>;

class Texture;
using TexturePtr = SharedPtr<Texture>;

enum class TextureFormat {
    NONE,
    RED,
    RG,

    RGB565,
    RGBA4444,
    RGBA5551,

    Alpha8,
    Luminance8,
    LuminanceAlpha8,
    RGB8,
    RGBA8,
    BGRA8,

    Alpha16,
    Luminance16,
    LuminanceAlpha16,
    RGB16,
    RGBA16,

    Alpha32,
    Luminance32,
    LuminanceAlpha32,
    RGB32,
    RGBA32,

    // Depth formats
    D16,
    D24,

    // Compressed formats
    PVRTC_RGB2,
    PVRTC_RGBA2,
    PVRTC_RGB4,
    PVRTC_RGBA4,

    PVRTC2_RGBA2,
    PVRTC2_RGBA4,

    ETC_RGB4,

    S3TC_DXT1,
    S3TC_DXT3,
    S3TC_DXT5,
};

class Texture : public Shared<Texture>
{
public:
    enum class FilterValue {
        NONE,
        NEAREST,
        LINEAR,
        NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR,
    };

    enum class WrapValue {
        NONE,
        CLAMP_TO_EDGE,
        MIRRORED_REPEAT,
        REPEAT,
    };

    enum class TargetType {
        TEXTURE_1D,
        TEXTURE_2D,
        TEXTURE_3D,
        TEXTURE_CUBE_MAP,
    };

public:
    Texture();
    Texture(const int width, const int height, const int glID);

    std::string getName() const;

    GPU_ID getGlID() const;
    void setGlID(GPU_ID value);

    int getWidth() const;
    int getHeight() const;
    void setSize(int width, int height);

    FilePtr getFile() const;
    void setFile(FilePtr &value);

    Uv getUv(const Vec2& p) const;
    FilterValue getMinFilter() const;
    void setMinFilter(const FilterValue &value);

    FilterValue getMagFilter() const;
    void setMagFilter(const FilterValue &value);

    WrapValue getWrapU() const;
    void setWrapU(const WrapValue &value);

    WrapValue getWrapV() const;
    void setWrapV(const WrapValue &value);

    TextureFormat getFormat() const;
    void setFormat(const TextureFormat &value);

    TargetType getTarget() const;
    void setTarget(const TargetType &value);

private:
    FilePtr file;
    std::string name;

    TargetType target;
    TextureFormat format;

    int width;
    int height;
    GPU_ID glID;

    FilterValue minFilter;
    FilterValue magFilter;
    WrapValue wrapU;
    WrapValue wrapV;
};

} // namespace re

#endif // TEXTURE_H
