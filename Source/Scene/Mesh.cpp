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

}
