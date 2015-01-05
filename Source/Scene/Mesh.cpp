#include "Mesh.h"

namespace re {

Mesh::Mesh()
{
    this->type = ComponentType::Mesh;
}

void Mesh::init()
{
    this->material = Material::create();
    this->material->initDefaultPass();
}

Geometry::ptr Mesh::getGeometry()
{
    return geometry;
}

void Mesh::setGeometry(const GeometryPtr &g)
{
    this->geometry = g;
}

MeshDataPtr Mesh::getMeshData() const
{
    return this->data;
}

void Mesh::setMeshData(MeshDataPtr data)
{
    this->data = data;
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
    ComponentPtr inst = Component::clone();

    return dynamic_pointer_cast<Mesh>(inst);
}

ComponentPtr Mesh::createCloneInstance() const
{
    return std::make_shared<Mesh>();
}

void Mesh::copyProperties(const Component *att)
{
    Component::copyProperties(att);

    const Mesh* inst = dynamic_cast<const Mesh*>(att);
    if (inst) {
        this->material = inst->material->clone();

        this->data = inst->data;
//        if (!inst->geometry->isStatic()) {
//            // only dynamic geometry need to be copy
//            this->geometry = inst->geometry;
//        } else {
//            this->geometry = inst->geometry;
//        }
    }
}

void re::ControlPoints::cacheVertex() {
    for (size_t i = 0; i < this->controlPoints.size(); ++i) {
      std::vector<uint> vertexIndexList;

      for (size_t j = 0; j < this->vertexToControl.size(); ++j) {
        uint &controlIndex = vertexToControl[j];

        if (controlIndex == i) {
          vertexIndexList.push_back(j);
        }
      }

      this->controlToVertex.push_back(vertexIndexList);
    }
}

VertexElement::VertexElement(VertexElementType type, AttributeFormat format, int size)
{
    this->type = type;
    this->format = format;
    this->size = size;
}

}
