#include "TextureUnitState.h"

#include <assert.h>

namespace re {

TextureUnitState::TextureUnitState()
{
    this->currentIndex = 0;
}

float TextureUnitState::getScaleV() const
{
    return scaleV;
}

void TextureUnitState::setScaleV(float value)
{
    scaleV = value;
}

void TextureUnitState::setUVstate(float offsetU, float offsetV, float scaleU, float scaleV, float rotation)
{
    this->offsetU = offsetU;
    this->offsetV = offsetV;
    this->scaleU = scaleU;
    this->scaleV = scaleV;
    this->rotation = rotation;
}

Texture::type TextureUnitState::getActivityTexture()
{
    assert(this->frames.size() > currentIndex);

    return this->frames[currentIndex];
}

void TextureUnitState::addTextureFrame(Texture::type texture)
{
    this->frames.push_back(texture);
}

float TextureUnitState::getScaleU() const
{
    return scaleU;
}

void TextureUnitState::setScaleU(float value)
{
    scaleU = value;
}

float TextureUnitState::getOffsetV() const
{
    return offsetV;
}

void TextureUnitState::setOffsetV(float value)
{
    offsetV = value;
}

float TextureUnitState::getOffsetU() const
{
    return offsetU;
}

void TextureUnitState::setOffsetU(float value)
{
    offsetU = value;
}

float TextureUnitState::getRotation() const
{
    return rotation;
}

void TextureUnitState::setRotation(float value)
{
    rotation = value;
}

}
