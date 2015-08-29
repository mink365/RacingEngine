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

ComponentPtr RenderElement::createCloneInstance() const
{
    return CreateCloneInstance<RenderElement>();
}

void RenderElement::copyProperties(const Component *component)
{
    Component::copyProperties(component);

    const RenderElement* inst = static_cast<const RenderElement*>(component);

    this->mesh = inst->mesh;
    this->material = inst->material;
}



} // namespace re

