#include "Material.h"
#include <memory>

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

Shader::ptr Material::getShader() const
{
    return shader;
}

void Material::setShder(Shader::ptr& value)
{
    shader = value;
}

std::vector<SamplerParameter::ptr> &Material::getSamplers()
{
    return samplers;
}

SamplerParameter::ptr Material::getSampler(const string &name)
{
    auto iter = std::find_if(samplers.begin(), samplers.end(), [&](SamplerParameter::ptr& item) {
        if (item->name == name) {
            return true;
        }

        return false;
    });

    if (iter != samplers.end()) {
        return *iter;
    }

    // TODO: some walkround
    if (!shader || !shader->getUniform(name)) {
        return nullptr;
    }

    auto parameter = SamplerParameter::create(name);
    this->samplers.push_back(parameter);

    return parameter;
}

void Material::addSampler(SamplerParameter::ptr rhs)
{
    this->samplers.push_back(rhs);
}

void Material::setTexture(const string &name, TexturePtr &tex)
{
    auto parameter = this->getSampler(name);

    parameter->setTexture(tex);
}

Material& Material::operator =(const Material &rhs)
{
    // DO NOT COPY PASSES!
    this->transparent = rhs.transparent;
    this->queueID = rhs.queueID;

    this->renderState = rhs.renderState;
    this->shader = rhs.shader;

    return *this;
}

//ComponentPtr Material::createCloneInstance() const
//{
//    return CreateCloneInstance<Material>();
//}

//void Material::copyProperties(const Component *rhs)
//{
//    Component::copyProperties(rhs);

//    const Material* inst = dynamic_cast<const Material*>(rhs);
//    if (inst) {
//        this->queueID = inst->queueID;
//        this->renderState = inst->renderState;
//        this->shader = inst->shader;
//        this->transparent = inst->transparent;

//        for (auto& pass : inst->passes) {
//            auto passInst = pass->clone();

//            this->addPass(passInst);
//        }

//        for (auto& param : inst->samplers) {
//            auto paramInst = param->clone();

//            this->samplers.push_back(paramInst);
//        }
//    }
//}

}
