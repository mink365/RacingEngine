#ifndef MATERIAL_H
#define MATERIAL_H

#include <render/RenderState.h>
#include "TextureUnitState.h"
#include <shader/Shader.h>

class Material
{
public:
    Material();

    RenderState& getRenderState();

    bool isTransparent() const;
    void setTransparent(bool value);

    int getQueueID() const;

    TextureUnitState& getTexture();

    Shader *getShder() const;
    void setShder(Shader *value);

private:
    bool transparent;
    int queueID;

    RenderState renderState;

    TextureUnitState texture;

    Shader *shder;
};

#endif // MATERIAL_H
