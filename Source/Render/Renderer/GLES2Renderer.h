#ifndef RE_GLES2RENDERER_H
#define RE_GLES2RENDERER_H

#include "Renderer.h"

namespace re {

class GLES2Renderer : public Renderer
{
public:
    GLES2Renderer();

    virtual void setViewPort(int x, int y, int width, int height);

    virtual void setTexture(int unit, bool enable, const Texture &texture);

    virtual void bindRenderTarget(const RenderTarget &target);
    virtual void resetRenderTarget();

    virtual void bindShader(const Shader &shader);
    virtual void bindBuffer(const Geometry &geometry);

    virtual void renderMesh(const Geometry &geometry);

    virtual void cleanBuffers(bool color, bool depth, bool stencil);
    virtual void cleanBuffers(int flag);

    virtual void applyRenderState(const RenderState &state, bool force=false);

protected:
    virtual void activateTextureUnit(int unit);

    void setupRenderTarget(RenderTarget &target);
};

} // namespace re

#endif // RE_GLES2RENDERER_H
