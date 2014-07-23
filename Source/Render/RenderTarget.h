#ifndef RE_RENDERTARGET_H
#define RE_RENDERTARGET_H

#include "PredefineType.h"

#include "Texture/Texture.h"

namespace re {

class RenderTarget
{
public:
    RenderTarget();

private:
    TexturePtr texture;

};

} // namespace re

#endif // RE_RENDERTARGET_H
