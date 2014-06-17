#include "Mesh.h"

namespace re {

Mesh::Mesh()
{
    this->type = NodeAttributeType::Mesh;
}

Geometry &Mesh::getGeometry()
{
    return geometry;
}

Material& Mesh::getMaterial()
{
    return material;
}

MeshPtr Mesh::clone()
{
    NodeAttributePtr inst = NodeAttribute::clone();

    return dynamic_pointer_cast<Mesh>(inst);
}

NodeAttributePtr Mesh::createCloneInstance() const
{
    return std::make_shared<Mesh>();
}

void Mesh::copyProperties(const NodeAttribute *att)
{
    NodeAttribute::copyProperties(att);

    const Mesh* inst = dynamic_cast<const Mesh*>(att);
    if (inst) {
        this->material = inst->material;
        this->geometry = inst->geometry;
    }
}

}
