#ifndef TEXTURE_H
#define TEXTURE_H

#include "platform.h"
#include <string>
#include "Math/UV.h"

namespace re {

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

    enum class InternalFormat {
        NONE,
        RED,
        RG,
        RGB,
        RGBA,
        RGBA16,
    };

    enum class PixelFormat {
        NONE,
        RED,
        RG,
        Alpha,
        Luminance,
        LuminanceAlpha,
        RGB,
        RGBA,
    };

    enum class DataType {
        NONE,
        UNSIGNED_BYTE,
        BYTE,
        UNSIGNED_SHORT,
        SHORT,
        UNSIGNED_INT,
        INT,
        FLOAT,
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

    InternalFormat getInternalFormat() const;
    void setInternalFormat(const InternalFormat &value);

    PixelFormat getPixelFormat() const;
    void setPixelFormat(const PixelFormat &value);

    DataType getDataType() const;
    void setDataType(const DataType &value);

private:
    FilePtr file;
    std::string name;

    int width;
    int height;
    GPU_ID glID;

    FilterValue minFilter;
    FilterValue magFilter;
    WrapValue wrapU;
    WrapValue wrapV;

    InternalFormat internalFormat;
    PixelFormat pixelFormat;
    DataType dataType;
};

} // namespace re

#endif // TEXTURE_H
