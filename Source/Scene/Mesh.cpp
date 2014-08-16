#include "Mesh.h"

namespace re {

Mesh::Mesh()
{
    this->type = NodeAttributeType::Mesh;
}

void Mesh::init()
{
    this->geometry = Geometry::create();

    this->material = Material::create();
    this->material->initDefaultPass();
}

Geometry::ptr Mesh::getGeometry()
{
    return geometry;
}

void Mesh::setGeometry(GeometryPtr &g)
{
    this->geometry = g;
}

Material::ptr Mesh::getMaterial()
{
    return material;
}

void Mesh::setMaterial(MaterialPtr &m)
{
    this->material = m;
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

        if (!inst->geometry->isStatic()) {
            // only dynamic geometry need to be copy
            this->geometry = inst->geometry;
        } else {
            this->geometry = inst->geometry;
        }
    }
}

}
