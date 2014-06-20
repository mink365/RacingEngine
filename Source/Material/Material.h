#ifndef MATERIAL_H
#define MATERIAL_H

#include "Render/RenderState.h"
#include "TextureUnitState.h"
#include "Shader/Shader.h"

namespace re {

class Material
{
public:
    Material();

    RenderState& getRenderState();

    bool isTransparent() const;
    void setTransparent(bool value);

    int getQueueID() const;

    TextureUnitState& getTexture();

    Shader::type getShder() const;
    void setShder(Shader::type &value);

private:
    bool transparent;
    int queueID;

    RenderState renderState;

    TextureUnitState texture;

    Shader::type shder;
};

} // namespace re

#endif // MATERIAL_H
