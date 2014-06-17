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

NodeAttributePtr Mesh::createCloneInstance()
{
    return std::make_shared<Mesh>();
}

void Mesh::copyProperties(NodeAttribute* att)
{
    NodeAttribute::copyProperties(att);

    Mesh* inst = dynamic_cast<Mesh*>(att);
    if (inst) {
        this->material = inst->material;
        this->geometry = inst->geometry;
    }
}

}
