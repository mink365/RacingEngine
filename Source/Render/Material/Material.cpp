#include "Material.h"

namespace re {

Material::Material()
{
    this->transparent = false;
    this->queueID = 50;
}

void Material::initDefaultPass()
{
    // init default pass and unit
    auto pass = Pass::create();
    this->addPass(pass);

    auto unit = TextureUnitState::create();
    pass->addTextureUnit(unit);
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

void Material::addPass(Pass::ptr &pass)
{
    this->passes.push_back(pass);
    pass->material = this->shared_from_this();
}

Pass::ptr Material::getPass(int index)
{
    return this->passes.at(index);
}

int Material::getPassCount() const
{
    return this->passes.size();
}

void Material::clearPasses()
{
    this->passes.clear();
}

RenderState& Material::getRenderState()
{
    return renderState;
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
    inst->transparent = this->transparent;

    for (auto pass : this->passes) {
        auto passInst = pass->clone();

        inst->addPass(passInst);
    }

    return inst;
}

}
