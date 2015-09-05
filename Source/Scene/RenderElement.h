#ifndef RE_RENDERELEMENT_H
#define RE_RENDERELEMENT_H

#include "Base/ECS/Component.h"

namespace re {

class Mesh;
class Material;

using MeshPtr = SharedPtr<Mesh>;
using MaterialPtr = SharedPtr<Material>;

class RenderElement : public Component<RenderElement>
{
    template<class T>
    friend class Component;
public:
    RenderElement();
    RenderElement(MaterialPtr material, MeshPtr mesh);

    MaterialPtr getMaterial() const;
    void setMaterial(const MaterialPtr &value);

    MeshPtr getMesh() const;
    void setMesh(const MeshPtr &value);

protected:
    virtual void copyProperties(const RenderElement& inst);

private:
    MaterialPtr material;
    MeshPtr mesh;
};

} // namespace re

#endif // RE_RENDERELEMENT_H
