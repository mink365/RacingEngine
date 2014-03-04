#ifndef MESH_H
#define MESH_H

#include "SceneNode.h"
#include <geometry/Geometry.h>
#include <material/Material.h>

class Mesh : public SceneNode
{
public:
    Mesh();

    Material& getMaterial();

    Geometry& getGeometry();

private:
    Material material;

    Geometry geometry;
};

#endif // MESH_H
