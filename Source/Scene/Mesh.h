#ifndef MESH_H
#define MESH_H

#include "SceneNode.h"
#include "Geometry/Geometry.h"
#include "Material/Material.h"
#include "NodeAttribute.h"

namespace re {

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

class Mesh : public NodeAttribute
{
public:
    Mesh();

    Material& getMaterial();

    Geometry& getGeometry();

private:
    Material material;

    Geometry geometry;
};

} // namespace re

#endif // MESH_H
