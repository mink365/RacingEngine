#include "RenderElement.h"
#include "Render/Mesh.h"
#include "Material/Material.h"

namespace re {

RenderElement::RenderElement()
{
    material = Create<Material>();
    mesh = Create<Mesh>();
}

RenderElement::RenderElement(MaterialPtr material, MeshPtr mesh)
    :material(material), mesh(mesh)
{

}
MaterialPtr RenderElement::getMaterial() const
{
    return material;
}

void RenderElement::setMaterial(const MaterialPtr &value)
{
    material = value;
}

MeshPtr RenderElement::getMesh() const
{
    return mesh;
}

void RenderElement::setMesh(const MeshPtr &value)
{
    mesh = value;
}

void RenderElement::copyProperties(const RenderElement& inst)
{
    this->mesh = inst.mesh;
    this->material = inst.material;
}



} // namespace re

