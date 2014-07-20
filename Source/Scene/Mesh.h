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

    /**
     * @brief init
     * init a material and geometry
     */
    void init();

    Material::ptr getMaterial();
    void setMaterial(MaterialPtr& m);

    Geometry::ptr getGeometry();

    MeshPtr clone();

protected:
    virtual NodeAttributePtr createCloneInstance() const override;
    virtual void copyProperties(const NodeAttribute* att) override;

private:
    Material::ptr material;

    Geometry::ptr geometry;
};

} // namespace re

#endif // MESH_H
