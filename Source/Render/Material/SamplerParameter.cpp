#include "SamplerParameter.h"

#include <assert.h>

namespace re {

SamplerParameter::SamplerParameter(const string& name)
{
    this->name = name;

    this->scale = Vec2(1, 1);
}

void SamplerParameter::setUVstate(const Vec2& offset, const Vec2& scale, float rotation)
{
    this->offset = offset;
    this->scale = scale;
    this->rotation = rotation;
}

const string &SamplerParameter::getName() const
{
    return this->name;
}

Texture::ptr SamplerParameter::getTexture()
{
    return texture;
}

void SamplerParameter::setTexture(TexturePtr &tex)
{
    RE_ASSERT(tex != nullptr);
    this->texture = tex;
}

const Vec2 &SamplerParameter::getOffset() const
{
    return this->offset;
}

void SamplerParameter::setOffset(const Vec2 &rhs)
{
    this->offset = rhs;
}

const Vec2 &SamplerParameter::getScale() const
{
    return this->scale;
}

void SamplerParameter::setScale(const Vec2 &rhs)
{
    this->scale = rhs;
}

float SamplerParameter::getRotation() const
{
    return rotation;
}

void SamplerParameter::setRotation(float value)
{
    rotation = value;
}

SamplerParameter::ptr SamplerParameter::clone() const
{
    SamplerParameter::ptr inst = SamplerParameter::create("");

    inst->name = this->name;
    inst->offset = this->offset;
    inst->scale = this->scale;
    inst->rotation = this->rotation;

    inst->texture = this->texture;

    return inst;
}

}
