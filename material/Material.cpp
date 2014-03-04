#include "Material.h"

Material::Material()
{
    this->transparent = false;
    this->queueID = 50;
}

bool Material::isTransparent() const
{
    return transparent;
}

void Material::setTransparent(bool value)
{
    transparent = value;
}

int Material::getQueueID() const
{
    return this->queueID;
}

RenderState& Material::getRenderState()
{
    return renderState;
}

TextureUnitState &Material::getTexture()
{
    return texture;
}

Shader *Material::getShder() const
{
    return shder;
}

void Material::setShder(Shader *value)
{
    shder = value;
}
