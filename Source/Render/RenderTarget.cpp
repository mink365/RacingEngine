#include "RenderTarget.h"

namespace re {

RenderTarget::RenderTarget()
{
    this->type = RenderTargetType::FLAT;

    this->size.set(Size(512, 512));

    this->framebuffer = 0;
    this->colorRenderBuffer = 0;
    this->depthRenderbuffer = 0;

    this->hasDepthBuffer = true;
    this->hasStencilBuffer = false;

    this->generateMipmaps = false;
}

RenderTargetType RenderTarget::getType() const
{
    return this->type;
}

void RenderTarget::setTexture(TexturePtr texture)
{
    this->texture = texture;
}

TexturePtr RenderTarget::getTexture() const
{
    return this->texture;
}

bool RenderTarget::getHasDepthBuffer() const
{
    return hasDepthBuffer;
}

void RenderTarget::setHasDepthBuffer(bool value)
{
    hasDepthBuffer = value;
}

bool RenderTarget::getHasStencilBuffer() const
{
    return hasStencilBuffer;
}

void RenderTarget::setHasStencilBuffer(bool value)
{
    hasStencilBuffer = value;
}

bool RenderTarget::getGenerateMipmaps() const
{
    return generateMipmaps;
}

void RenderTarget::setGenerateMipmaps(bool value)
{
    generateMipmaps = value;
}

Size RenderTarget::getSize() const
{
    return size;
}

void RenderTarget::setSize(const Size &value)
{
    size = value;
}

RenderTargetCube::RenderTargetCube()
{
    this->type = RenderTargetType::CUBE;

    this->activeCubeFace = 0;
}

} // namespace re
