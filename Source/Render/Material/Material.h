#ifndef MATERIAL_H
#define MATERIAL_H

#include "Base/Named.h"
#include "Base/Clonable.h"
#include "Render/RenderState.h"
#include "SamplerParameter.h"
#include "Shader/Shader.h"
#include "Pass.h"

namespace re {

class Material : public Shared<Material>, public Named, public std::enable_shared_from_this<Material>
{
public:
    Material();

    void initDefaultPass();

    RenderState& getRenderState();

    bool isTransparent() const;
    void setTransparent(bool value);

    int getQueueID() const;
    void setQueueID(int id);

    void addPass(Pass::ptr& pass);
    Pass::ptr getPass(int index);
    int getPassCount() const;
    void clearPasses();

    Shader::ptr getShader() const;
    void setShder(Shader::ptr &value);

    std::vector<SamplerParameter::ptr>& getSamplers();
    SamplerParameter::ptr getSampler(const string& name);
    void addSampler(SamplerParameter::ptr rhs);
    void setTexture(const string& name, TexturePtr& tex);

    Material& operator =(const Material& rhs);

private:
    bool transparent;
    int queueID;

    RenderState renderState;

    std::vector<Pass::ptr> passes;
    std::vector<SamplerParameter::ptr> samplers;

    Shader::ptr shader;
};

} // namespace re

#endif // MATERIAL_H
