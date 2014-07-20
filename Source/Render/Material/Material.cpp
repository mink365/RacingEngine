#include "Material.h"

namespace re {

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

void Material::setQueueID(int id)
{
    this->queueID = id;
}

RenderState& Material::getRenderState()
{
    return renderState;
}

TextureUnitState &Material::getTexture()
{
    return texture;
}

Shader::ptr Material::getShder() const
{
    return shader;
}

void Material::setShder(Shader::ptr& value)
{
    shader = value;
}

Material::ptr Material::clone() const
{
    Material::ptr inst = Material::create();

    inst->queueID = this->queueID;
    inst->renderState = this->renderState;
    inst->shader = this->shader;
    inst->texture = this->texture;
    inst->transparent = this->transparent;

    return inst;
}

}
