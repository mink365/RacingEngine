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

    MeshPtr clone();

protected:
    virtual NodeAttributePtr createCloneInstance() override;
    virtual void copyProperties(NodeAttribute* att) override;

private:
    // TODO: use shared_ptr instance of a data
    Material material;

    // TODO: geometry not need to be copy? or we just copy it in deep clone?
    Geometry geometry;
};

} // namespace re

#endif // MESH_H
