#include "TextureUnitState.h"

#include <assert.h>

namespace re {

TextureUnitState::TextureUnitState()
{
}

void TextureUnitState::setUVstate(float offsetU, float offsetV, float scaleU, float scaleV, float rotation)
{
    this->offset.x = offsetU;
    this->offset.y = offsetV;
    this->scale.x = scaleU;
    this->scale.y = scaleV;
    this->rotation = rotation;
}

Texture::ptr TextureUnitState::getTexture()
{
    return texture;
}

void TextureUnitState::setTexture(TexturePtr &tex)
{
    RE_ASSERT(tex != nullptr);
    this->texture = tex;
}

float TextureUnitState::getRotation() const
{
    return rotation;
}

void TextureUnitState::setRotation(float value)
{
    rotation = value;
}

const Vec2 &TextureUnitState::getOffset() const
{
    return this->offset;
}

void TextureUnitState::setOffset(float u, float v)
{
    this->offset.set(u, v);
}

const Vec2 &TextureUnitState::getScale() const
{
    return this->scale;
}

void TextureUnitState::setScale(float u, float v)
{
    this->scale.set(u, v);
}

TextureUnitState::ptr TextureUnitState::clone() const
{
    TextureUnitState::ptr inst = TextureUnitState::create();

    inst->offset = this->offset;
    inst->scale = this->scale;
    inst->rotation = this->rotation;
    inst->pass.reset();

    inst->texture = this->texture;

    return inst;
}

}
