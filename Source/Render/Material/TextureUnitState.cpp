#include "TextureUnitState.h"

#include <assert.h>

namespace re {

TextureUnitState::TextureUnitState()
{
    this->activeFrameIndex = 0;
}

void TextureUnitState::setUVstate(float offsetU, float offsetV, float scaleU, float scaleV, float rotation)
{
    this->offset.x = offsetU;
    this->offset.y = offsetV;
    this->scale.x = scaleU;
    this->scale.y = scaleV;
    this->rotation = rotation;
}

Texture::ptr TextureUnitState::getActivityTexture()
{
    assert(this->frames.size() > activeFrameIndex);

    return this->frames[activeFrameIndex];
}

void TextureUnitState::addTextureFrame(Texture::ptr texture)
{
    this->frames.push_back(texture);
}

TextureUnitState::ptr TextureUnitState::clone() const
{
    TextureUnitState::ptr inst = TextureUnitState::create();

    inst->offset = this->offset;
    inst->scale = this->scale;
    inst->rotation = this->rotation;
    inst->pass.reset();
    inst->activeFrameIndex = 0;

    inst->frames = this->frames;

    return inst;
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

}
