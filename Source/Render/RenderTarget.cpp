#include "RenderTarget.h"

namespace re {

RenderTarget::RenderTarget()
{
    this->type = RenderTargetType::FLAT;

    this->generateMipmaps = true;
    this->hasDepthBuffer = true;
    this->hasStencilBuffer = true;
}

RenderTarget::~RenderTarget()
{

}

RenderTargetType RenderTarget::getType() const
{
    return this->type;
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

RenderTargetCube::RenderTargetCube()
{
    this->type = RenderTargetType::CUBE;

    this->activeCubeFace = 0;
}

} // namespace re
