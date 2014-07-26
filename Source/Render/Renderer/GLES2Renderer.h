#ifndef RE_GLES2RENDERER_H
#define RE_GLES2RENDERER_H

#include "Renderer.h"

namespace re {

class GLES2Renderer : public Renderer
{
public:
    GLES2Renderer();

    virtual void setViewport(const Rect& viewport);

    virtual void setTexture(int unit, bool enable, const Texture &texture);

    virtual void bindRenderTarget(const RenderTarget &target);
    virtual void resetRenderTarget();

    virtual void bindShader(const Shader &shader);
    virtual void bindBuffer(const Geometry &geometry);

    virtual void renderMesh(const Geometry &geometry);

    virtual void cleanBuffers(bool color, bool depth, bool stencil);
    virtual void cleanBuffers(int flag);

    virtual void applyRenderState(const RenderState &state, bool force=false);

    virtual void setupRenderTarget(RenderTarget &target);
protected:
    virtual void activateTextureUnit(int unit);
};

} // namespace re

#endif // RE_GLES2RENDERER_H
