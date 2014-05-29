#include "Mesh.h"

namespace re {

Mesh::Mesh()
{
    this->type = MESH;
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
