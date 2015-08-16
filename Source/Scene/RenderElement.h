#ifndef RE_RENDERELEMENT_H
#define RE_RENDERELEMENT_H

#include "PreDeclare.h"
#include "Component.h"

namespace re {

class RenderElement : public Component
{
public:
    RenderElement();
    RenderElement(MaterialPtr material, MeshPtr mesh);

    MaterialPtr getMaterial() const;
    void setMaterial(const MaterialPtr &value);

    MeshPtr getMesh() const;
    void setMesh(const MeshPtr &value);

private:
    MaterialPtr material;
    MeshPtr mesh;
};

} // namespace re

#endif // RE_RENDERELEMENT_H
