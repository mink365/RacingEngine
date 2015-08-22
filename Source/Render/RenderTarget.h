#ifndef RE_RENDERTARGET_H
#define RE_RENDERTARGET_H

#include "PreDeclare.h"

#include "Texture/Texture.h"
#include "Math/Rect.h"

namespace re {

enum class RenderTargetType {
    FLAT,
    CUBE,
};

class RenderTarget
{
public:
    RenderTarget();
    virtual ~RenderTarget() {};

    RenderTargetType getType() const;

    Size getSize() const;
    void setSize(const Size &value);

    void setTexture(TexturePtr texture);
    TexturePtr getTexture() const;

    bool getHasDepthBuffer() const;
    void setHasDepthBuffer(bool value);

    bool getHasStencilBuffer() const;
    void setHasStencilBuffer(bool value);

    bool getGenerateMipmaps() const;
    void setGenerateMipmaps(bool value);

public:
    GPU_ID framebuffer;
    GPU_ID renderbuffer;

    RenderTargetPtr shareDepthFrom;

protected:
    Size size;

    // TODO: MRT
    TexturePtr texture;

    RenderTargetType type;

    bool hasDepthBuffer;
    bool hasStencilBuffer;
    bool generateMipmaps;
};

class RenderTargetCube : public RenderTarget {
public:
    RenderTargetCube();

public:
    GPU_ID framebuffers[6];
    GPU_ID renderbuffers[6];

    int activeCubeFace;
};

} // namespace re

#endif // RE_RENDERTARGET_H
