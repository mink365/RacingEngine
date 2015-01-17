#ifndef MATERIAL_H
#define MATERIAL_H

#include "Scene/Component.h"
#include "Render/RenderState.h"
#include "TextureUnitState.h"
#include "Shader/Shader.h"
#include "Pass.h"

namespace re {

class Material : public Component, public enable_shared_from_this<Material>
{
public:
    Material();
    void init() {};

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

    Material& operator =(const Material& rhs);

    MaterialPtr clone();

protected:
    virtual ComponentPtr createCloneInstance() const override;
    virtual void copyProperties(const Component* rhs) override;

private:
    bool transparent;
    int queueID;

    RenderState renderState;

    std::vector<Pass::ptr> passes;

    Shader::ptr shader;
};

} // namespace re

#endif // MATERIAL_H
