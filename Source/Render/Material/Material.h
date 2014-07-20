#ifndef MATERIAL_H
#define MATERIAL_H

#include "Base/Shared.h"
#include "Base/Named.h"
#include "Base/Clonable.h"
#include "Render/RenderState.h"
#include "TextureUnitState.h"
#include "Shader/Shader.h"
#include "Pass.h"

namespace re {

class Material : public Shared<Material>,
                 public Clonable<Material>,
                 public enable_shared_from_this<Material>
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

    Shader::ptr getShder() const;
    void setShder(Shader::ptr &value);

    Material::ptr clone() const override;

private:
    bool transparent;
    int queueID;

    RenderState renderState;

    std::vector<Pass::ptr> passes;

    Shader::ptr shader;
};

} // namespace re

#endif // MATERIAL_H
