#ifndef MATERIAL_H
#define MATERIAL_H

#include "Base/Shared.h"
#include "Base/Named.h"
#include "Base/Clonable.h"
#include "Render/RenderState.h"
#include "TextureUnitState.h"
#include "Shader/Shader.h"

namespace re {

class Material : public Shared<Material>, public Clonable<Material>
{
public:
    Material();

    RenderState& getRenderState();

    bool isTransparent() const;
    void setTransparent(bool value);

    int getQueueID() const;
    void setQueueID(int id);

    TextureUnitState& getTexture();

    Shader::ptr getShder() const;
    void setShder(Shader::ptr &value);

    Material::ptr clone() const override;

private:
    bool transparent;
    int queueID;

    RenderState renderState;

    TextureUnitState texture;

    Shader::ptr shader;
};

} // namespace re

#endif // MATERIAL_H
